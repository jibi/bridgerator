/*
	    DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
		    Version 2, December 2004

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

	    DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. You just DO WHAT THE FUCK YOU WANT TO.
*/

#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>

#include <cstring>
#include <unistd.h>

#include <bridgerator/config.h>
#include <bridgerator/server.h>
#include <bridgerator/socket_engine.h>

#include <bridgerator/listener.h>
#include <bridgerator/client.h>

#include <bridgerator/socks5.h>

namespace bridgerator {

server *server::singleton = NULL;

void
server::init()
{
	singleton = new server;

	singleton->init_listeners();
}

void
server::loop()
{
	singleton->epoll_loop();

}

server::server()
{

}

void
server::init_listeners()
{
	for (int i = 0; i < (int) config::get().services_count(); i++) {
		int sd;
		listener *l;
		service  s;
		
		s = config::get().service_at(i);
		l = new listener(s.local_port, s.remote_address, s.remote_port);
		sd = l->start();

		if (sd == -1) {
			std::cout << "Failed to bind on port " << config::get().service_at(i).local_port << std::endl;
			continue;
		}

		listeners[sd] = l;
		e.add_socket(sd);
	}
}

void
server::handle_new_client(int sd)
{
	listener  *l;
	client    *c;

	struct sockaddr_in client_addr;
	int c_sd;
	int f_sd;

	l = listeners[sd];
	c_sd = l->do_accept(&client_addr);

	if (c_sd == -1) {
		std::cout << "Failed to accept new connection" << std::endl;
		return;
	}

	f_sd = socks5::connect_socks_proxy(config::get().proxy_address(), config::get().proxy_port(),
					   l->remote_address(), l->remote_port());

	if (f_sd == -1) {
		std::cout << "Failed to connect to remote host" << std::endl;
		return;
	}

	c = new client(c_sd, f_sd);

	clients[c_sd] = c;
	servers[f_sd] = c;

	e.add_socket(c_sd);
	e.add_socket(f_sd);
}

void
server::remove_client(client *c)
{
	int c_sd;
	int s_sd;

	c_sd = c->c_sd();
	s_sd = c->s_sd();

	delete c;
	clients.erase(c_sd);
	servers.erase(s_sd);

	e.del_socket(c_sd);
	e.del_socket(s_sd);
}

void
server::epoll_loop()
{
	struct epoll_event events[max_events];

	while (1) {
		int ret = e.do_wait(events);

		for (int i = 0; i < ret; i++) {
			int sd = events[i].data.fd;

			if (listeners.count(sd)) {
				handle_new_client(sd);

			} else if (clients.count(sd)) {
				client *c = clients[sd];

				if (! c->client_to_server_fwd()) {
					remove_client(c);

				}
			} else if (servers.count(sd)) {
				client *c = servers[sd];

				if (! c->server_to_client_fwd()) {
					remove_client(c);
				}

			} else {
				/* no reason this branch should be reached. */
			}
		}
	}
}
}

