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

#include <bridgerator/listener.h>
#include <cstring>

using namespace std;

namespace bridgerator {

listener::listener(uint16_t local_port, string remote_address, uint16_t remote_port)
{

	_remote_address = remote_address;
	_remote_port    = remote_port;

	memset(&_addr, '0', sizeof(_addr));

	_addr.sin_family = AF_INET;

	/* TODO: select the address to bind on */
	_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	_addr.sin_port        = htons(local_port);
}

int
listener::start()
{
	if((_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		return -1;
	}

	if (bind(_socket, (struct sockaddr *)  &_addr, sizeof(_addr)) == -1) {
		return -1;
	}
		
	if (listen(_socket, 20) == -1) {
		return -1;
	}

	return _socket;
}

int
listener::do_accept(struct sockaddr_in *client)
{
	socklen_t len;

	return accept(_socket, (struct sockaddr *) client, &len);
}

string
listener::remote_address()
{
	return _remote_address;
}

uint16_t
listener::remote_port()
{
	return _remote_port;
}

}
