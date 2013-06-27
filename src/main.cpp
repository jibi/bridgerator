#include<iostream>
#include<unordered_map>

#include<bridgerator/config>
#include<bridgerator/epoll_manager>
#include<bridgerator/network>

#include<unistd.h>

config *bt_config;
std::unordered_map<int, listener  *> listeners;
std::unordered_map<int, client    *> clients;
std::unordered_map<int, forwarder *> forwarders;

int main() {
	bt_config = new config();
	epoll_manager e;
	struct epoll_event events[100];

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

	while (1) {
		int ret = e.do_wait(events);

		for (int i = 0; i < ret; i++) {
			int fd = events[i].data.fd;

			if (listeners.count(fd)) {
				listener  *l;
				client    *c;
				forwarder *f;

				struct sockaddr_in client_addr;
				int c_sd;
				int f_sd;

				l = listeners[fd];
				c_sd = l->do_accept(&client_addr);

				if (c_sd == -1) {
					std::cout << "Failed to accept new connection" << std::endl;
					continue;
				}

				c = new client(c_sd);
				clients[c_sd] = c;

				e.add_socket(c_sd);
			} else if (clients.count(fd)) {
				client *c;

				c = clients[fd];
				if (! c->read_and_forward()) {
					delete c;
					clients.erase(fd);

					e.del_socket(fd);
				}

			} else if (forwarders.count(fd)) {

			} else {
				/* no reason this branch should be reached. */
			}
		}
	}
}

