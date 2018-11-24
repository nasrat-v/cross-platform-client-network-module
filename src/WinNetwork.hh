#pragma once

#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <string>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <io.h>
#include <vector>

# define NET_ERROR		(ERR)-1
# define SUCCESS		(ERR)1
# define SIZE_BUFF		4096

typedef short			ERR;

class WinNetwork
{
public:
	WinNetwork();
	~WinNetwork();

	typedef struct		s_serverParam
	{
		PCSTR			ipAddr; // ip address of server, if NULL set hostname - ex: 127.0.0.1 - can be NULL if hostName != NULL - priority if both are set
		PCSTR			hostName; // hostname of server only if ip address is unknown - ex: chat.freenode.net - can be NULL if ipAddr != NULL
		PCSTR			serviceName; // only if hostname is set - ex: http - can be NULL
		short			port; // port of server - ex: 6667 - can't be NULL
		int				ipType; // ip type used by server - ex:: AF_INET for IPV4 or AF_INET6 for IPV6 - can't be NULL
		int				socketType; // socket type used by server - ex: SOCK_STREAM - can't be NULL
		IPPROTO			protocol; // ip protocol used by server - ex: IPPROTO_TCP for TCP protocol - can't be NULL
		const char		*endCharSrv; // end character when sending msg - ex: \r\n for IRC - can be NULL
	}					t_serverParam;

	ERR					initNetworkClient(const t_serverParam &srvParam);
	ERR					connectToServer();
	void				deconnectToServer();
	ERR					readData(std::string &data);
	ERR					writeData(const std::string &data);
	bool				isDataToRead();
	void				clearSocket();


private:
	/* Attributes */
	SOCKET						_sock;
	std::vector<SOCKADDR_IN>	_sins;
	WSADATA						_wsaData;
	fd_set						_fd_set;
	struct addrinfo				*_addrInfo;
	t_serverParam				_srvParam;

	/* Methods */
	ERR					initSocket();
	ERR					initHandleSocketWithIpAddress();
	void				initHandleSocketWithHostname();
	ERR					findIpAddrWithHostname();
};

