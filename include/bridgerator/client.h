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

#ifndef _BRIDGERATOR_CLIENT_H
#define _BRIDGERATOR_CLIENT_H

#include <unistd.h>

namespace bridgerator {

class client {
public:
	client(int socket, int forwarder_socket);
	bool client_to_server_fwd();
	bool server_to_client_fwd();

	int c_sd();
	int s_sd();
private:
	int _c_sd;
	int _s_sd;

	static const size_t read_size = 4096;

	bool do_fwd(int src_fd, int dst_fd);
};

}

#endif

