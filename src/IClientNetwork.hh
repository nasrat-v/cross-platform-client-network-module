#pragma once

#ifdef _WIN32
	#include <winsock2.h>
	#include <Ws2tcpip.h>
#elif __linux__
	// Implements linux library
#endif

#include <iostream>
#include <string>
#include <vector>
#include <io.h>

# define NET_ERROR		(ERR)~0
# define SUCCESS		(ERR)1
# define SIZE_BUFF		4096

typedef short			ERR;

class IClientNetwork
{
public:
	virtual ~IClientNetwork() { }

	typedef struct		s_serverParam
	{
		const char		*ipAddr; // ip address of server, if NULL set hostname - ex: 127.0.0.1 - can be NULL if hostName != NULL - priority if both are set
		const char		*hostName; // hostname of server only if ip address is unknown - ex: chat.freenode.net - can be NULL if ipAddr != NULL
		const char		*serviceName; // only if hostname is set - ex: http - can be NULL
		short			port; // port of server - ex: 6667 - can't be NULL
		int				ipType; // ip type used by server - ex:: AF_INET for IPV4 or AF_INET6 for IPV6 - can't be NULL
		int				socketType; // socket type used by server - ex: SOCK_STREAM - can't be NULL
		int				protocol; // ip protocol used by server - ex: IPPROTO_TCP for TCP protocol - can't be NULL
		const char		*endCharSrv; // end character when sending msg - ex: \r\n for IRC - can be NULL
	}					t_serverParam;

	virtual ERR					initNetworkClient(const t_serverParam &srvParam) = 0;
	virtual ERR					connectToServer() = 0;
	virtual void				deconnectToServer() = 0;
	virtual ERR					readData(std::string &data) = 0;
	virtual ERR					writeData(const std::string &data) = 0;
	virtual bool				isDataToRead() = 0;

protected:
	/* Attributes */
#ifdef _WIN32
	SOCKET						_sock;
	std::vector<SOCKADDR_IN>	_sins;
	WSADATA						_wsaData;
#elif __linux__
	// Implements linux attributes
#endif

	fd_set						_fd_set;
	struct addrinfo				*_addrInfo;
	t_serverParam				_srvParam;

	/* Methods */
	virtual ERR					initSocket() = 0;
	virtual ERR					initHandleSocketWithIpAddress() = 0;
	virtual void				initHandleSocketWithHostname() = 0;
	virtual ERR					findIpAddrWithHostname() = 0;
	virtual void				clearSocket() = 0;
};

