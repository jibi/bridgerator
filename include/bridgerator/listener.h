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

#ifndef _BRIDGERATOR_LISTENER_H
#define _BRIDGERATOR_LISTENER_H

#include <string>
#include <netinet/in.h>

using namespace std;

namespace bridgerator {

class listener {
public:
	listener(uint16_t local_port, string remote_address, uint16_t remote_port);
	int start();
	int do_accept(struct sockaddr_in *client);

	string remote_address();
	unsigned short remote_port();

private:
	int                _socket;
	struct sockaddr_in _addr;

	string             _remote_address;
	unsigned short     _remote_port;
};

}

#endif

