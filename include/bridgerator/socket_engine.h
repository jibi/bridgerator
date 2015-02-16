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

#ifndef _BRIDGERATOR_SOCKET_ENGINE_H
#define _BRIDGERATOR_SOCKET_ENGINE_H

#include <sys/epoll.h>

namespace bridgerator {

const int max_events = 64;

class socket_engine {
private:
	int _epfd;
	int _fd_count;

public:
	socket_engine();
	int add_socket(int fd);
	int del_socket(int fd);
	int do_wait(struct epoll_event *events);
};
}

#endif

