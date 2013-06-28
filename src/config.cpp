#include<iostream>
#include<vector>
#include<sstream>
#include<fstream>

#include<unistd.h>
#include<sys/types.h>
#include<pwd.h>

#include<bridgerator/config>

std::string
config::read_config_file() {
	struct passwd *pw = getpwuid(getuid());
	std::ifstream fs_config(std::string(pw->pw_dir) + "/.bridgerator");

	if (!fs_config.is_open()) {
		std::cout << "Cannot open ~/.bridgerator config file, bay :(" << std::endl;
		exit(1);
	}

	std::string content((std::istreambuf_iterator<char>(fs_config)),
	  (std::istreambuf_iterator<char>()));

	fs_config.close();
	return content;
}

void
config::parse_server(std::string value) {
	std::istringstream is_value(value);
	std::string field;

	std::getline(is_value, field, ':');
	_proxy_address = field;

	std::getline(is_value, field);
	_proxy_port = std::atoi(field.c_str());
}

void
config::parse_service(std::string value) {
	std::istringstream is_value(value);
	std::string field;
	service s;

	std::getline(is_value, field, ':');
	s.local_port = std::atoi(field.c_str());

	std::getline(is_value, field, ':');
	s.remote_address = field;

	std::getline(is_value, field);
	s.remote_port = std::atoi(field.c_str());

	if (s.remote_address.empty() || s.local_port == 0 || s.remote_port == 0)
		return;

	_services.push_back(s);
}

void
config::parse_config_file(std::string content) {
	std::istringstream is_line(content);
	std::string line;

	while(std::getline(is_line, line)) {
		std::istringstream is_key(line);
		std::string key, value;

		if(std::getline(is_key, key, ' ')) {
			if(std::getline(is_key, value)) {
				if (key == "server") {
					parse_server(value);
				} else if (key == "bind") {
					parse_service(value);
				}
			}
		}
	}

}

config::config() {
	std::string content = read_config_file();
	parse_config_file(content);
}

std::string
config::proxy_address() {
	return _proxy_address;
}

unsigned short
config::proxy_port() {
	return _proxy_port;
}

std::size_t
config::services_count() {
	return _services.size();
}

service const
config::service_at(std::size_t pos) {
	return _services.at(pos);
}

