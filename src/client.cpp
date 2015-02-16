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

#include <cerrno>
#include <unistd.h>

#include <bridgerator/client.h>

namespace bridgerator {

client::client(int c_sd, int s_sd)
{
	_c_sd = c_sd;
	_s_sd = s_sd;
}

bool
client::client_to_server_fwd()
{
	return do_fwd(_c_sd, _s_sd);
}

bool
client::server_to_client_fwd()
{
	return do_fwd(_s_sd, _c_sd);
}

bool
client::do_fwd(int src_fd, int dst_fd)
{
	char buf[read_size];
	ssize_t count;

	do {
		count = read(src_fd, buf, read_size);
	} while (count == -1 && errno == EINTR);

	if (count <= 0) {
		return false;
	}

	/*
	 * TODO: make sure to send all data
	 */
	write(dst_fd, buf, count);

	return true;
}

int
client::c_sd()
{
	return _c_sd;
}

int
client::s_sd()
{
	return _s_sd;
}

}
