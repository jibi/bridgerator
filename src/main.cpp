#include<iostream>
#include<bridgerator/config>

config *bt_config;

int main() {
	bt_config = new config();

	for (int i = 0; i < bt_config->services_count(); i++) {
		/* start listeners */
	}

	while (1) {
		/* epoll handling */
	}
}

