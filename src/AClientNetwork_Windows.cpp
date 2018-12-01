
#include "AClientNetwork_Windows.hh"

AClientNetwork_Windows::AClientNetwork_Windows()
{
	std::cout << "WSA Startup" << std::endl << std::endl;
	WSAStartup(MAKEWORD(2, 0), &_wsaData);
}

AClientNetwork_Windows::~AClientNetwork_Windows()
{
	deconnectToServer();
	std::cout << "WSA Cleanup" << std::endl;
	WSACleanup();
}

ERR AClientNetwork_Windows::initSocket()
{
	if ((_sock = socket(_srvParam.ipType, _srvParam.socketType, _srvParam.protocol)) == INVALID_SOCKET)
	{
		std::cerr << "Error socket initialization" << std::endl;
		return (NET_ERROR);
	}
	std::cout << "Successfully initialize socket: " << _sock << std::endl;
	return (SUCCESS);
}

ERR AClientNetwork_Windows::initHandleSocketWithIpAddress()
{
	ULONG binaryIpAddress = 0;
	SOCKADDR_IN sin;

	memset(&_sins, 0, sizeof(_sins));
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = _srvParam.ipType;
	sin.sin_port = htons(_srvParam.port);
	if (InetPtonA(_srvParam.ipType, _srvParam.ipAddr, &binaryIpAddress) != 1)
	{
		std::cout << "Error when initialize socket, invalid param: " << WSAGetLastError() << std::endl;
		return (NET_ERROR);
	}
	sin.sin_addr.s_addr = binaryIpAddress;
	_sins.push_back(sin);
	std::cout << "Successfully initialize socket handle with ip address" << std::endl;
	return (SUCCESS);
}

void AClientNetwork_Windows::initHandleSocketWithHostname()
{
	int i = 0;
	SOCKADDR_IN sin;
	struct addrinfo *p;

	memset(&_sins, 0, sizeof(_sins));
	for (p = _addrInfo; p != NULL; p = p->ai_next)
	{
		memset(&sin, 0, sizeof(sin));
		sin = *(reinterpret_cast<SOCKADDR_IN*>(p->ai_addr));
		sin.sin_port = htons(_srvParam.port);
		_sins.push_back(sin);
		i++;
	}
	std::cout << i << " ip address found" << std::endl;
	if (_addrInfo != NULL)
		freeaddrinfo(_addrInfo);
	std::cout << "Successfully initialize socket handle with hostname" << std::endl;
}

ERR AClientNetwork_Windows::findIpAddrWithHostname()
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

ERR AClientNetwork_Windows::initNetworkClient(const t_serverParam &srvParam)
{
	_srvParam = srvParam;
	_sins.clear();
	std::cout << "Server param received" << std::endl << "Launch initialization of the client..." << std::endl << std::endl;
	if (_srvParam.ipAddr != NULL)
		initHandleSocketWithIpAddress();
	else if (_srvParam.hostName != NULL)
	{
		if (findIpAddrWithHostname() == NET_ERROR)
			return(NET_ERROR);
		initHandleSocketWithHostname();
	}
	else
	{
		std::cout << "Bad param - no ip address or hostname set" << std::endl;
		return (NET_ERROR);
	}
	if (initSocket() == NET_ERROR)
		return (NET_ERROR);
	return (SUCCESS);
}

ERR AClientNetwork_Windows::connectToServer()
{
	char hostsIp[SIZE_BUFF] = { 0 };
	std::string connectionData;

	for (SOCKADDR_IN &sin : _sins)
	{
		if (InetNtopA(sin.sin_family, &sin.sin_addr, hostsIp, SIZE_BUFF) == NULL)
			std::cout << "Warning ! error when parsing ip server" << std::endl;
		else
			std::cout << std::endl << "Trying to connect to server " << hostsIp << ':' << sin.sin_port << std::endl;
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

void AClientNetwork_Windows::deconnectToServer()
{
	char error_code[SIZE_BUFF] = { 0 };
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
		
ERR AClientNetwork_Windows::readData(std::string &data)
{
	int ret;
	char buff[(SIZE_BUFF + sizeof(char))] = { 0 };

	if ((ret = recv(_sock, buff, SIZE_BUFF, 0)) == SOCKET_ERROR)
	{
		std::cerr << "Error failed to read data from socket" << std::endl;
		return (NET_ERROR);
	}
	buff[ret] = '\0'; // to be sure
	data = std::string(buff);
	std::cout << "Received: " << std::endl << data << std::endl << ":From Server" << std::endl << std::endl;
	return (SUCCESS);
}

ERR AClientNetwork_Windows::writeData(const std::string &data)
{
	int size = (int)(data.size() + strlen(_srvParam.endCharSrv));
	const char *dataToSend = (data + _srvParam.endCharSrv).c_str();

	if ((send(_sock, dataToSend, size, 0)) == SOCKET_ERROR)
	{
		std::cerr << "Error failed to write data to socket" << std::endl;
		return (NET_ERROR);
	}
	std::cout << "Send:" << std::endl << data << std::endl << ":To server" << std::endl << std::endl;
	return (SUCCESS);
}

bool AClientNetwork_Windows::isDataToRead()
{
	clearSocket();
	if (select(((int)_sock + 1), &_fd_set, NULL, NULL, NULL) == -1)
	{
		std::cerr << "Error on select with code: " << WSAGetLastError() << std::endl;
		return (false);
	}
	return (FD_ISSET(_sock, &_fd_set));
}

void AClientNetwork_Windows::clearSocket()
{
	FD_ZERO(&_fd_set);
	FD_SET(_sock, &_fd_set);
	std::cout << "Socket: " << _sock << " cleared" << std::endl << std::endl;
}
