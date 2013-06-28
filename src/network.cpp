#include<iostream>
#include<unordered_map>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>

#include<string.h>
#include<unistd.h>

#include<bridgerator/config>
#include<bridgerator/network>
#include<bridgerator/epoll_manager>
#include<bridgerator/socks5>

extern config *bt_config;
extern std::unordered_map<int, listener *>  listeners;
extern std::unordered_map<int, client *>    clients;
extern std::unordered_map<int, forwarder *> forwarders;
extern epoll_manager e;

listener::listener(unsigned short local_port, std::string remote_address,
	  unsigned short remote_port) {

	_remote_address = remote_address;
	_remote_port    = remote_port;

	memset(&_addr, '0', sizeof(_addr));

	_addr.sin_family      = AF_INET;
	_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_addr.sin_port        = htons(local_port);
}

int
listener::start() {
	if((_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return -1;

	if (bind(_socket, (struct sockaddr *)  &_addr, sizeof(_addr)) == -1)
		return -1;
		
	if (listen(_socket, 20) == -1)
		return -1;

	return _socket;
}

int
listener::do_accept(struct sockaddr_in *client) {
	socklen_t len;

	return accept(_socket, (struct sockaddr *) client, &len);
}

std::string
listener::remote_address() {
	return _remote_address;
}

unsigned short
listener::remote_port() {
	return _remote_port;
}

client::client(int socket, int forwarder_socket) {
	_socket = socket;
	_forwarder_socket = forwarder_socket;
}

bool
client::read_and_forward() {
	char buf[1024];
	ssize_t count;

	do {
		count = read(_socket, buf, 1024);
	} while (count == -1 && errno == EINTR);

	if (count == 0 || count == -1) {
		return false;
	}

	write(_forwarder_socket, buf, count);

	return true;
}

forwarder::forwarder(int socket, int client_socket) {
	_socket = socket;
	_client_socket = client_socket;
}

bool
forwarder::read_and_deliver() {
	char buf[1024];
	ssize_t count;

	do {
		count = read(_socket, buf, 1024);
	} while (count == -1 && errno == EINTR);

	if (count == 0 || count == -1) {
		return false;
	}

	write(_client_socket, buf, count);

	return true;
}

void
init_listeners() {
	for (int i = 0; i < bt_config->services_count(); i++) {
		int sd;
		listener *l;
		service  s;
		
		s = bt_config->service_at(i);
		l = new listener(s.local_port, s.remote_address, s.remote_port);
		sd = l->start();

		if (sd == -1) {
			std::cout << "Failed to bind on port " << bt_config->service_at(i).local_port << std::endl;
			continue;
		}

		listeners[sd] = l;
		e.add_socket(sd);
	}
}

void
handle_new_client(int sd) {
	listener  *l;
	client    *c;
	forwarder *f;

	struct sockaddr_in client_addr;
	int c_sd;
	int f_sd;

	l = listeners[sd];
	c_sd = l->do_accept(&client_addr);

	if (c_sd == -1) {
		std::cout << "Failed to accept new connection" << std::endl;
		return;
	}

	f_sd = socks5::connect_socks_proxy(bt_config->proxy_address(), bt_config->proxy_port(),
	  l->remote_address(), l->remote_port());

	if (f_sd == -1) {
		std::cout << "Failed to connect to remote host" << std::endl;
		return;
	}

	c = new client(c_sd, f_sd);
	f = new forwarder(f_sd, c_sd);

	clients[c_sd]    = c;
	forwarders[f_sd] = f;

	e.add_socket(c_sd);
	e.add_socket(f_sd);
}

void
epoll_loop() {
	struct epoll_event events[100];

	while (1) {
		int ret = e.do_wait(events);

		for (int i = 0; i < ret; i++) {
			int fd = events[i].data.fd;

			if (listeners.count(fd)) {
				handle_new_client(fd);

			} else if (clients.count(fd)) {
				client *c;

				c = clients[fd];
				if (! c->read_and_forward()) {
					delete c;
					clients.erase(fd);

					/* TODO get fw */

					e.del_socket(fd);
				}
			} else if (forwarders.count(fd)) {
				forwarder *f;

				f = forwarders[fd];
				if (! f->read_and_deliver()) {
					delete f;
					forwarders.erase(fd);

					e.del_socket(fd);
				}

			} else {
				/* no reason this branch should be reached. */
			}
		}
	}
}

