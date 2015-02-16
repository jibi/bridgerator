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

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <bridgerator/socks5.h>

namespace bridgerator {
namespace socks5 {

namespace proto {
	namespace utils {
		size_t
		append_req(uint8_t *req, size_t pos, uint8_t *what, size_t len)
		{
			memcpy(req + pos, what, len);
			return pos + len;
		}

		size_t
		append_req(uint8_t *req, size_t pos, uint8_t what)
		{
			req[pos] = what;
			return pos + 1;
		}
	}
}

int
handshake(int sd)
{
	uint8_t req[3] = { proto::ver, 1, proto::method::no_auth };
	uint8_t res[2];
	ssize_t count;

	write(sd, req, 3);

	count = read(sd, res, 2);
	if (count != 2 || res[1] == proto::method::no_accettable) {
		return -1;
	}

	return 0;
}

int
connect(int sd, const char *address, uint16_t port)
{
	uint8_t  req[255 + 8];
	uint8_t  res[255 + 8];
	size_t         pos = 0;
	struct in_addr ip_addr;

	pos = proto::utils::append_req(req, pos, proto::ver);
	pos = proto::utils::append_req(req, pos, proto::cmd::connect);
	pos = proto::utils::append_req(req, pos, proto::res);

	if (inet_aton(address, &ip_addr) == 0) {
		size_t len = strlen(address);

		pos = proto::utils::append_req(req, pos, proto::atyp::domainname);
		pos = proto::utils::append_req(req, pos, (uint8_t) len);
		pos = proto::utils::append_req(req, pos, (uint8_t *) address, len);

	} else { /* TODO: check for ipv6 */
		pos = proto::utils::append_req(req, pos, proto::atyp::ipv4);
		pos = proto::utils::append_req(req, pos, (uint8_t *) &ip_addr.s_addr, 4);
	}

	port = htons(port);
	pos = proto::utils::append_req(req, pos, (uint8_t *) &port, 2);

	write(sd, req, pos);
	read(sd, res, 255 + 8);

	if (res[1] != proto::result::succeded) {
		std::cout << "err: " << (int) res[1] << std::endl;
		return -1;
	}

	/* TODO: extract remote address and port */
	return 0;
}

int
connect_socks_proxy(std::string proxy_address, uint16_t proxy_port, std::string remote_address, uint16_t remote_port)
{
	struct sockaddr_in addr;
	struct in_addr ip_addr;

	int sd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&addr, '0', sizeof(addr));
	inet_aton(proxy_address.c_str(), &ip_addr);

	addr.sin_family  = AF_INET;
	addr.sin_port    = ntohs(proxy_port);
	addr.sin_addr    = ip_addr;

	connect(sd, (struct sockaddr *) &addr, sizeof(addr));
	if (socks5::handshake(sd) == 0) {
		if (socks5::connect(sd, remote_address.c_str(), remote_port) == socks5::proto::result::succeded) {
			return sd;
		}
	}

	return -1;
}
}
}
