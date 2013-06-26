#include<iostream>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>

#include<string.h>
#include<unistd.h>

#include<bridgerator/network>

listener::listener(unsigned short port) {
	memset(&_addr, '0', sizeof(_addr));

	_addr.sin_family      = AF_INET;
	_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_addr.sin_port        = htons(port);
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

client::client(int socket) {
	_socket = socket;
}

forwarder *
client::forwarder() {
	return _forwarder;
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

	//forward

	std::cout << buf << std::endl;

	return true;
}

forwarder::forwarder(int socket) {
	_socket = socket;
}

int
forwarder::socket() {
	return _socket;
}
