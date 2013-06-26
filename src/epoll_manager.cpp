#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#include<bridgerator/epoll_manager>

static const int max_events = 64;

epoll_manager::epoll_manager() {
	_epfd     = epoll_create(max_events);
	_fd_count = 0;
}

int
epoll_manager::add_socket(int fd) {
	struct epoll_event event;

	event.data.fd = fd;
	event.events = EPOLLIN;

	_fd_count++;
	return epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &event);
}

int
epoll_manager::del_socket(int fd) {
	return epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL);
}

int
epoll_manager::do_wait(struct epoll_event *events) {
	return epoll_wait(_epfd, events, _fd_count, -1);
}
