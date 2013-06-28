#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>

#include<iostream>
#include<bridgerator/socks5>

namespace bridgerator {
namespace socks5 {

namespace proto {
	namespace utils {
		size_t
		append_req(unsigned char *req, size_t pos, unsigned char *what, size_t len) {
			memcpy(req + pos, what, len);
			return pos + len;
		}

		size_t
		append_req(unsigned char *req, size_t pos, unsigned char what) {
			req[pos] = what;
			return pos + 1;
		}
	}
}

int
handshake(int sd) {
	unsigned char req[3] = { proto::ver, 1, proto::method::no_auth };
	unsigned char res[2];
	ssize_t count;

	write(sd, req, 3);

	count = read(sd, res, 2);
	if (count != 2 || res[1] == proto::method::no_accettable) {
		return -1;
	}

	return 0;
}

int
connect(int sd, const char *address, unsigned short port) {
	unsigned char  req[255 + 8];
	unsigned char  res[255 + 8];
	size_t         pos = 0;
	struct in_addr ip_addr;

	pos = proto::utils::append_req(req, pos, proto::ver);
	pos = proto::utils::append_req(req, pos, proto::cmd::connect);
	pos = proto::utils::append_req(req, pos, proto::res);

	if (inet_aton(address, &ip_addr) == 0) {
		size_t len = strlen(address);

		pos = proto::utils::append_req(req, pos, proto::atyp::domainname);
		pos = proto::utils::append_req(req, pos, (unsigned char) len);
		pos = proto::utils::append_req(req, pos, (unsigned char *) address, len);

	} else { /* TODO: check for ipv6 */
		pos = proto::utils::append_req(req, pos, proto::atyp::ipv4);
		pos = proto::utils::append_req(req, pos, (unsigned char *) &ip_addr.s_addr, 4);
	}

	port = htons(port);
	pos = proto::utils::append_req(req, pos, (unsigned char *) &port, 2);

	write(sd, req, pos);
	read(sd, res, 255 + 8);

	if (res[1] == proto::result::succeded) {
		/* TODO: extract remote address and port */
		return 0;
	} else {
		std::cout << "err: " << (int) res[1] << std::endl;
	}

	return -1;
}

int
connect_socks_proxy(std::string proxy_address, unsigned short proxy_port,
	  std::string remote_address, unsigned short remote_port) {
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
