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

#include <cstdlib>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#include <bridgerator/socket_engine.h>

namespace bridgerator {

socket_engine::socket_engine()
{
	_epfd     = epoll_create(max_events);
	_fd_count = 0;
}

int
socket_engine::add_socket(int fd)
{
	struct epoll_event event;

	event.data.fd = fd;
	event.events = EPOLLIN;

	_fd_count++;
	return epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &event);
}

int
socket_engine::del_socket(int fd)
{
	_fd_count--;
	return epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL);
}

int
socket_engine::do_wait(struct epoll_event *events)
{
	return epoll_wait(_epfd, events, _fd_count, -1);
}
}
