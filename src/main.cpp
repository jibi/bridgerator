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

#include <iostream>
#include <unordered_map>

#include <bridgerator/config.h>
#include <bridgerator/socket_engine.h>
#include <bridgerator/server.h>
#include <bridgerator/socks5.h>

using namespace std;
using namespace bridgerator;

void
bai(std::string reason)
{
	std::cout << reason << std::endl;
	exit(1);
}

int
main()
{
	try {
		config::init();
	} catch (exception e) {
		/* FIXME: */
		bai(std::string("Cannot init bridgerator config:\n") + std::string(e.what()));
	}

	server::init();
	server::loop();

	return 0;
}
