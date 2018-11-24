
#include "WinNetwork.hh"

WinNetwork::WinNetwork()
{
	std::cout << "WSA Startup" << std::endl << std::endl;
	WSAStartup(MAKEWORD(2, 0), &_wsaData);
}

WinNetwork::~WinNetwork()
{
	deconnectToServer();
	std::cout << "WSA Cleanup" << std::endl;
	WSACleanup();
}

ERR WinNetwork::initSocket()
{
	if ((_sock = socket(_srvParam.ipType, _srvParam.socketType, _srvParam.protocol)) == INVALID_SOCKET)
	{
		std::cerr << "Error socket initialization" << std::endl;
		return (NET_ERROR);
	}
	std::cout << "Successfully initialize socket" << std::endl;
	return (SUCCESS);
}

void WinNetwork::initHandleSocket()
{
	int i = 0;
	SOCKADDR_IN sin;
	struct addrinfo *p;

	memset(&_sins, 0, sizeof(_sins));
	for (p = _addrInfo; p != NULL; p = p->ai_next)
	{
		sin = *(reinterpret_cast<SOCKADDR_IN*>(p->ai_addr));
		sin.sin_port = htons(_srvParam.port);
		_sins.push_back(sin);
		i++;
	}
	std::cout << i << " ip address found" << std::endl;
	if (_addrInfo != NULL)
		freeaddrinfo(_addrInfo);
	std::cout << "Successfully initialize socket handle" << std::endl;
}

ERR WinNetwork::findIpAddrWithHostname()
{
	struct addrinfo	hints;

	memset(&_addrInfo, 0, sizeof(_addrInfo));
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = _srvParam.ipType;
	hints.ai_socktype = _srvParam.socketType;
	hints.ai_protocol = _srvParam.protocol;
	if ((GetAddrInfoA(_srvParam.hostName, _srvParam.serviceName, &hints, &_addrInfo) != 0) || (_addrInfo == NULL))
	{
		std::cerr << "Error cannot retrieve ip address with hostname" << std::endl;
		return (NET_ERROR);
	}
	std::cout << "Successfully retrieve ip address of: " << _srvParam.hostName << std::endl;
	return (SUCCESS);
}

ERR WinNetwork::initNetworkClient(const t_serverParam &srvParam)
{
	_srvParam = srvParam;
	std::cout << "Server param received" << std::endl << "Launch initialization of the client..." << std::endl << std::endl;
	if (findIpAddrWithHostname() == NET_ERROR)
		return(NET_ERROR);
	initHandleSocket();
	if (initSocket() == NET_ERROR)
		return (NET_ERROR);
	return (SUCCESS);
}

ERR WinNetwork::connectToServer()
{
	char hosts[SIZE_BUFF];
	std::string connectionData;

	for (SOCKADDR_IN &sin : _sins)
	{
		InetNtopA(sin.sin_family, &sin.sin_addr, hosts, SIZE_BUFF);
		std::cout << std::endl << "Trying to connect to server " << hosts << ':' << sin.sin_port << std::endl;
		if (connect(_sock, (SOCKADDR*)&sin, sizeof(SOCKADDR)) != SOCKET_ERROR)
		{
			std::cout << "Successfully connect to server" << std::endl << std::endl;
			return (SUCCESS);
		}
		std::cerr << "Error connection failed with code: " << WSAGetLastError() << std::endl;
	}
	std::cerr << std::endl << "Error all connections failed" << std::endl;
	return (NET_ERROR);
}

void WinNetwork::deconnectToServer()
{
	char error_code[SIZE_BUFF];
	int error_code_size = sizeof(error_code);
	int status;

	std::cout << std::endl << "Cleaning server connection..." << std::endl;
	if (getsockopt(_sock, SOL_SOCKET, SO_ERROR, error_code, &error_code_size) == SOCKET_ERROR)
	{
		if ((status = WSAGetLastError()) == WSAENOTSOCK)
			std::cout << "Socket already closed or never initialized" << std::endl;
		else
			std::cout << "Error when closing socket: " << status << std::endl;
	}
	else
	{
		std::cout << "Successfully close socket" << std::endl;
		closesocket(_sock);
	}
}
		
ERR WinNetwork::readData(std::string &data)
{
	int ret;
	char buff[(SIZE_BUFF + sizeof(char))];

	memset(buff, 0, (SIZE_BUFF + sizeof(char)));
	if ((ret = recv(_sock, buff, SIZE_BUFF, 0)) == SOCKET_ERROR)
	{
		std::cerr << "Error failed to read data from socket" << std::endl;
		return (NET_ERROR);
	}
	buff[ret] = '\0';
	data = std::string(buff);
	return (SUCCESS);
}

ERR WinNetwork::writeData(const std::string &data)
{
	int size = (int)(data.size() + strlen(_srvParam.endCharSrv));
	const char *dataToSend = (data + _srvParam.endCharSrv).c_str();

	if ((send(_sock, dataToSend, size, 0)) == SOCKET_ERROR)
	{
		std::cerr << "Error failed to write data to socket" << std::endl;
		return (NET_ERROR);
	}
	std::cout << "send: " << data << _srvParam.endCharSrv << " :to server" << std::endl;
	return (SUCCESS);
}

bool WinNetwork::isDataToRead()
{
	if (select(((int)_sock + 1), &_fd_set, NULL, NULL, NULL) == -1)
	{
		std::cerr << "Error on select" << std::endl;
		return (false);
	}
	return (FD_ISSET(_sock, &_fd_set));
}

void WinNetwork::clearSocket()
{
	FD_ZERO(&_fd_set);
	FD_SET(_sock, &_fd_set);
}
