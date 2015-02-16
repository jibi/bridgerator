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

#ifndef _BRIDGERATOR_CONFIG_H
#define _BRIDGERATOR_CONFIG_H

#include <string>
#include <vector>

using namespace std;

namespace bridgerator {

typedef struct {
	uint16_t local_port;
	string   remote_address;
	uint16_t remote_port;
} service;

class config {
public:
	static void init();
	static config& get();

	string proxy_address();
	uint16_t proxy_port();
	size_t services_count();
	service const service_at(size_t pos);

private:
	static config *singleton;

	string          _proxy_address;
	uint16_t        _proxy_port;
	vector<service> _services;

	config();
	config(const config&) = delete;
	config& operator=(const config&) = delete;

	string read_config_file();
	void parse_server(string value);
	void parse_service(string value);
	void parse_config_file(string content);
};

}

#endif

