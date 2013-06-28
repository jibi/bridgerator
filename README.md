bridgerator
=============

A socks5 bridge/client/whatever.

Usage
-------

Place in your home dir a file .bridgerator like this

```
server address_of_socks_proxy:port_of_socks_proxy

bind local_port_0:remote_address_0:remote_port_0
bind local_port_1:remote_address_1:remote_port_1
```

then connect to localhost:local_port_n


Problems
-------
At the moment it binds to 0.0.0.0.

