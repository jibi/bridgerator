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

#ifndef _BRIDGERATOR_NETWORK_H
#define _BRIDGERATOR_NETWORK_H

#include <iostream>
#include <unordered_map>

#include <sys/epoll.h>

#include <bridgerator/socket_engine.h>
#include <bridgerator/listener.h>
#include <bridgerator/client.h>

namespace bridgerator {

class server {
public:
	static void init();
	static void loop();

private:
	static server *singleton;

	std::unordered_map<int, listener *>  listeners;

	/*
	 * map a client socket fd to its client class
	 */
	std::unordered_map<int, client *>    clients;

	/*
	 * map a server fd to the requesting client
	 */
	std::unordered_map<int, client *>    servers;

	socket_engine e;

	server();
	server(const server&) = delete;
	server& operator=(const server&) = delete;
	void init_listeners();
	void epoll_loop();

	void remove_client(client *c);
	void handle_new_client(int sd);
};

}

#endif

