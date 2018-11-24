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
		PCSTR			hostName; // ex: chat.freenode.net
		PCSTR			serviceName; // ex: http
		short			port; // ex: 6667
		int				ipType; // ex:: AF_INET for IPV4 or AF_INET6 for IPV6
		int				socketType; // ex: SOCK_STREAM
		IPPROTO			protocol; // ex: IPPROTO_TCP for TCP protocol
		const char		*endCharSrv; // end character when sending msg, ex: for IRC \r\n
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
	void				initHandleSocket();
	ERR					findIpAddrWithHostname();
};

