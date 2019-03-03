
#ifndef __ICLIENTNETWORK__
#define __ICLIENTNETWORK__

#ifdef _WIN32
	#include <winsock2.h>
	#include <Ws2tcpip.h>
	#include <io.h>
#elif __linux__
    #include <arpa/inet.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <stdio.h>
#endif

#include <iostream>
#include <string>
#include <vector>

# define NET_ERROR		(ERR)~0
# define SIZE_BUFF		4096
#endif

typedef short           ERR;

class IClientNetwork
{
public:
	virtual ~IClientNetwork() = default;

    typedef struct		s_serverParam
	{
		const char		*ipAddr; // ip address of server, if NULL set hostname - ex: 127.0.0.1 - can be NULL if hostName != NULL - priority if both are set
		const char		*hostName; // hostname of server only if ip address is unknown - ex: chat.freenode.net - can be NULL if ipAddr != NULL
		const char		*serviceName; // only if hostname is set - ex: http - can be NULL
		short			port; // port of server - ex: 6667 - can't be NULL
		int				socketType; // socket type used by server - ex: SOCK_STREAM - can't be NULL
		int				protocol; // ip protocol used by server - ex: IPPROTO_TCP for TCP protocol - can't be NULL
#ifdef _WIN32
        int				ipType; // ip type used by server - ex:: AF_INET for IPV4 or AF_INET6 for IPV6 - can't be NULL
#elif __linux__
        sa_family_t     ipType; // ip type used by server - ex:: AF_INET for IPV4 or AF_INET6 for IPV6 - can't be NULL
#endif
	}					t_serverParam;

	virtual ERR					initNetworkClient(const t_serverParam &srvParam) = 0;
	virtual ERR					connectToServer() = 0;
	virtual void				deconnectToServer() = 0;
	virtual ERR					readData(std::string &data) = 0;
	virtual ERR					writeData(const std::string &data) = 0;
	virtual bool				isDataToRead() = 0;
	virtual bool				isConnected() const = 0;

protected:
	/* Attributes */
#ifdef _WIN32
	SOCKET						    _sock;
	std::vector<SOCKADDR_IN>	    _sins;
	WSADATA						    _wsaData;
#elif __linux__
	int                             _sock;
	std::vector<struct sockaddr_in> _sins;
#endif

	fd_set						_fd_set;
	struct addrinfo				*_addrInfo;
	t_serverParam				_srvParam;
	bool						_connected;

	/* Methods */
	virtual ERR					initSocket() = 0;
	virtual ERR					initHandleSocketWithIpAddress() = 0;
	virtual void				initHandleSocketWithHostname() = 0;
	virtual ERR					findIpAddrWithHostname() = 0;
	virtual void				clearSocket() = 0;
};

#endif // !__ICLIENTNETWORK__