#include<iostream>
#include<unordered_map>

#include<bridgerator/config>
#include<bridgerator/network>

config *bt_config;
std::unordered_map<int, listener  *> listeners;

int main() {
	bt_config = new config();

	for (int i = 0; i < bt_config->services_count(); i++) {
		int sd;
		listener *l;
		
		l = new listener(bt_config->service_at(i).local_port);
		sd = l->start();

		if (sd == -1) {
			std::cout << "Failed to bind on port " << bt_config->service_at(i).local_port << std::endl;
			continue;
		}

		listeners[sd] = l;
	}

	while (1) {
		/* epoll handling */
	}
}

