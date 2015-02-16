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

#ifndef _BRIDGERATOR_SOCKS_H
#define _BRIDGERATOR_SOCKS_H

#include <iostream>

namespace bridgerator {
namespace socks5 {
namespace proto {
	const uint8_t ver = 0x05;
	const uint8_t res = 0x00;

	namespace method {
		const uint8_t no_auth       = 0x00;
		const uint8_t no_accettable = 0xff;
	}

	namespace cmd {
		const uint8_t connect = 0x01;
	}

	namespace atyp {
		const uint8_t ipv4       = 0x01;
		const uint8_t domainname = 0x03;
	}

	namespace result {
		const uint8_t succeded           = 0x00;
		const uint8_t server_failure     = 0x01;
		const uint8_t conn_not_allowed   = 0x02;
		const uint8_t host_unreachable   = 0x03;
		const uint8_t conn_refused       = 0x04;
		const uint8_t ttl_expired        = 0x05;
		const uint8_t cmd_not_supported  = 0x06;
		const uint8_t addr_not_supported = 0x07;
	}

	namespace utils {
		size_t append_req(uint8_t *req, size_t pos, uint8_t *what, size_t len);
		size_t append_req(uint8_t *req, size_t pos, uint8_t what);
	}
}

int handshake(int sd);
int connect(int sd, const char *address, uint16_t port);
int connect_socks_proxy(std::string proxy_address, uint16_t proxy_port, std::string remote_address, uint16_t remote_port);

}
}

#endif

