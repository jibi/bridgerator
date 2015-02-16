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
#include <vector>
#include <sstream>
#include <fstream>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include <stdexcept>

#include <bridgerator/config.h>

using namespace std;

namespace bridgerator {

config *config::singleton = NULL;

void
config::init()
{
	singleton = new config;
}

config &
config::get()
{
	return *singleton;
}

string
config::read_config_file()
{
	struct passwd *pw = getpwuid(getuid());
	ifstream fs_config(string(pw->pw_dir) + "/.bridgerator");

	if (!fs_config.is_open())
		throw runtime_error("cannot open config file");

	string content((istreambuf_iterator<char>(fs_config)), (istreambuf_iterator<char>()));

	fs_config.close();
	return content;
}

void
config::parse_server(string value)
{
	istringstream is_value(value);
	string field;

	getline(is_value, field, ':');
	_proxy_address = field;

	getline(is_value, field);
	_proxy_port = atoi(field.c_str());
}

void
config::parse_service(string value)
{
	istringstream is_value(value);
	string field;
	service s;

	getline(is_value, field, ':');
	s.local_port = atoi(field.c_str());

	getline(is_value, field, ':');
	s.remote_address = field;

	getline(is_value, field);
	s.remote_port = atoi(field.c_str());

	if (s.remote_address.empty() || s.local_port == 0 || s.remote_port == 0)
		return;

	_services.push_back(s);
}

void
config::parse_config_file(string content)
{
	istringstream is_line(content);
	string line;

	while(getline(is_line, line)) {
		istringstream is_key(line);
		string key, value;

		if(getline(is_key, key, ' ')) {
			if(getline(is_key, value)) {
				if (key == "server") {
					parse_server(value);
				} else if (key == "bind") {
					parse_service(value);
				}
			}
		}
	}
}

config::config()
{
	string content = read_config_file();
	parse_config_file(content);
}

string
config::proxy_address()
{
	return _proxy_address;
}

uint16_t
config::proxy_port()
{
	return _proxy_port;
}

size_t
config::services_count()
{
	return _services.size();
}

service const
config::service_at(size_t pos)
{
	return _services.at(pos);
}

}
