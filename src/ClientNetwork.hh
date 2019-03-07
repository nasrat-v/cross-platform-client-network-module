
#ifndef __CLIENTNETWORK__
#define __CLIENTNETWORK__

#ifdef _WIN32
	#include <winsock2.h>
	#include <Ws2tcpip.h>
	#include <io.h>
#elif __linux__ || __unix__ || __unix || unix || __APPLE__ || __MACH__
    #include <arpa/inet.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <stdio.h>
	#include <cstring>
#endif

#include <iostream>
#include <string>
#include <vector>

#include "Log.hh"

#ifdef _WIN32
	# define __inetpton                 InetPtonA
    # define __inetntop                 InetNtopA
    # define __getaddrinfo              GetAddrInfoA
    # define __read_socket(a, b, c, d)  recv(a, b, c, d)
    # define __write_socket(a, b, c, d) send(a, b, c, d)
    # define __close_socket             closesocket
#elif __linux__ || __unix__ || __unix || unix || __APPLE__ || __MACH__
	# define __inetpton	                inet_pton
    # define __inetntop                 inet_ntop
    # define __getaddrinfo              getaddrinfo
    # define __read_socket(a, b, c, d)  read(a, b, c)
    # define __write_socket(a, b, c, d) write(a, b, c)
    # define __close_socket             close
#endif

# define NET_ERROR		(ERR)~0
# define SIZE_BUFF		4096

#ifdef _WIN32
    typedef int                 __ret;
    typedef int                 __size;
    typedef int                 __err_size
    typdef  ULONG               __binary_iptype;
    typedef SOCKET              __socket;
    typedef struct SOCKADDR     __sockaddr;
	typedef struct SOCKADDR_IN  __sockaddr_in;
#elif __linux__ || __unix__ || __unix || unix || __APPLE__ || __MACH__
    typedef ssize_t             __ret;
    typedef size_t              __size;
    typedef socklen_t           __err_size;
    typedef in_addr_t           __binary_iptype;
	typedef int                 __socket;
    typedef struct sockaddr     __sockaddr;
	typedef struct sockaddr_in  __sockaddr_in;
#endif

class ClientNetwork
{
public:
	ClientNetwork();
	~ClientNetwork();

    typedef struct		s_serverParam
	{
		const char		*ipAddr; // ip address of server, if NULL set hostname - ex: 127.0.0.1 - can be NULL if hostName != NULL - priority if both are set
		const char		*hostName; // hostname of server only if ip address is unknown - ex: chat.freenode.net - can be NULL if ipAddr != NULL
		const char		*serviceName; // only if hostname is set - ex: http - can be NULL
		uint16_t		port; // port of server - ex: 6667 - can't be NULL
		int				socketType; // socket type used by server - ex: SOCK_STREAM - can't be NULL
		int				protocol; // ip protocol used by server - ex: IPPROTO_TCP for TCP protocol - can't be NULL
#ifdef _WIN32
        int				ipType; // ip type used by server - ex:: AF_INET for IPV4 or AF_INET6 for IPV6 - can't be NULL
#elif __linux__ || __unix__ || __unix || unix || __APPLE__ || __MACH__
        sa_family_t     ipType; // ip type used by server - ex:: AF_INET for IPV4 or AF_INET6 for IPV6 - can't be NULL
#endif
	}					t_serverParam;

	ERR					initNetworkClient(const t_serverParam &srvParam);
	ERR					connectToServer();
	void				deconnectToServer();
	ERR					readData(std::string &data);
	ERR					writeData(const std::string &data);
	bool				isDataToRead();
	bool				isConnected() const;

protected:
	/* Attributes */
#ifdef _WIN32
	WSADATA						 _wsaData;
#endif
	__socket                    _sock;
	std::vector<__sockaddr_in>  _sins;
	fd_set				        _fd_set;
	struct addrinfo		        *_addrInfo;
	t_serverParam		        _srvParam;
	bool				        _connected;

	/* Methods */
#ifdef _WIN32
	void				startWSA();
	void				stopWSA();
#endif
	ERR					initSocket();
	ERR					initHandleSocketWithIpAddress();
	void				initHandleSocketWithHostname();
	ERR					findIpAddrWithHostname();
	void				clearSocket();
    int                 logFailureMsg(const std::string &msg, bool errorCode);
};

#endif // !__CLIENTNETWORK__