#include<iostream>
#include<unordered_map>

#include<bridgerator/config>
#include<bridgerator/epoll_manager>
#include<bridgerator/network>
#include<bridgerator/socks5>

#include<unistd.h>

using namespace bridgerator;

namespace bridgerator {
config *bt_config;
std::unordered_map<int, listener *>  listeners;
std::unordered_map<int, client *>    clients;
std::unordered_map<int, forwarder *> forwarders;
epoll_manager e;
}

int main() {
	bt_config = new config();
	init_listeners();

	epoll_loop();

	return 0;
}
