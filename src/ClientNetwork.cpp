
#include "../../header/Network/ClientNetwork.hh"

ClientNetwork::ClientNetwork()
{
	_connected = false;
}

ClientNetwork::~ClientNetwork() = default;

#ifdef _WIN32
void ClientNetwork::startWSA()
{
	WSAStartup(MAKEWORD(2, 0), &_wsaData);
	Log::logSomething("\nInitialize windows network - WSA Startup\n");
}

void ClientNetwork::stopWSA()
{
	WSACleanup();
	Log::logSomething("\nClean windows network - WSA Cleanup\n");
}
#endif

ERR ClientNetwork::initSocket()
{
	if ((_sock = socket(_srvParam.ipType, _srvParam.socketType, _srvParam.protocol)) == NET_ERROR)
	{
		LogNetwork::logFailureMsg("Error socket initialization");
		return (NET_ERROR);
	}
	LogNetwork::logSuccessMsg("Successfully initialize socket: " + std::to_string(_sock));
	return (SUCCESS);
}

ERR ClientNetwork::initHandleSocketWithIpAddress()
{
    __binary_iptype binaryIpAddress = 0;
	__sockaddr_in sin;

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = _srvParam.ipType;
	sin.sin_port = htons(_srvParam.port);
	if (__inetpton(_srvParam.ipType, _srvParam.ipAddr, &binaryIpAddress) != 1)
	{
		logFailureMsg("Error when initialize socket, invalid param ", true);
		return (NET_ERROR);
	}
	sin.sin_addr.s_addr = binaryIpAddress;
	_sins.push_back(sin);
	LogNetwork::logSuccessMsg("Successfully initialize socket handle with ip address");
	return (SUCCESS);
}

void ClientNetwork::initHandleSocketWithHostname()
{
	int i = 0;
    __sockaddr_in sin;
	struct addrinfo *p;

	for (p = _addrInfo; p != nullptr; p = p->ai_next)
	{
		memset(&sin, 0, sizeof(sin));
		sin = *(reinterpret_cast<__sockaddr_in*>(p->ai_addr));
		sin.sin_port = htons(_srvParam.port);
		_sins.push_back(sin);
		i++;
	}
	LogNetwork::logInfoMsg(std::to_string(i) + " ip address found");
	if (_addrInfo != nullptr)
		freeaddrinfo(_addrInfo);
	LogNetwork::logSuccessMsg("Successfully initialize socket handle with hostname");
}

ERR ClientNetwork::findIpAddrWithHostname()
{
	struct addrinfo	hints;

	memset(&_addrInfo, 0, sizeof(_addrInfo));
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = _srvParam.ipType;
	hints.ai_socktype = _srvParam.socketType;
	hints.ai_protocol = _srvParam.protocol;
	if ((__getaddrinfo(_srvParam.hostName, _srvParam.serviceName, &hints, &_addrInfo) != 0) || (_addrInfo == nullptr))
	{
		LogNetwork::logFailureMsg("Error cannot retrieve ip address with hostname");
		return (NET_ERROR);
	}
	LogNetwork::logSuccessMsg("Successfully retrieve ip address of: " + std::string(_srvParam.hostName));
	return (SUCCESS);
}

ERR ClientNetwork::initNetworkClient(const t_serverParam &srvParam)
{
#ifdef __WIN32
	startWSA();
#endif
	_srvParam = srvParam;
	_sins.clear();
	LogNetwork::logSomething("Server param received\nLaunch initialization of the client...\n");
	if (_srvParam.ipAddr != nullptr)
		initHandleSocketWithIpAddress();
	else if (_srvParam.hostName != nullptr)
	{
		if (findIpAddrWithHostname() == NET_ERROR)
			return (NET_ERROR);
		initHandleSocketWithHostname();
	}
	else
	{
		LogNetwork::logFailureMsg("Bad param - no ip address or hostname set");
		return (NET_ERROR);
	}
	if (initSocket() == NET_ERROR)
		return (NET_ERROR);
	return (SUCCESS);
}

ERR ClientNetwork::connectToServer()
{
	char hostsIp[SIZE_BUFF] = { 0 };
	std::string connectionData;

	for (__sockaddr_in &sin : _sins)
	{
		if (__inetntop(sin.sin_family, &sin.sin_addr, hostsIp, SIZE_BUFF) == nullptr)
			LogNetwork::logSomething("\nWarning ! error when parsing ip server");
		else
		    LogNetwork::logSomething("\nTrying to connect to server " + std::string(hostsIp) + ':' + std::to_string(sin.sin_port));
		if (connect(_sock, (__sockaddr*)&sin, sizeof(__sockaddr)) != NET_ERROR)
		{
			LogNetwork::logSuccessMsg("Successfully connect to server");
			_connected = true;
			return (SUCCESS);
		}
		logFailureMsg("Error connection failed ", true);
	}
	LogNetwork::logFailureMsg("Error all connections failed");
	return (NET_ERROR);
}

void ClientNetwork::deconnectToServer()
{
	char error_code[SIZE_BUFF] = { 0 };
	__err_size error_code_size = sizeof(error_code);
    __attribute__((unused))int status;

	LogNetwork::logTryMsg("Cleaning server connection...");
	if (getsockopt(_sock, SOL_SOCKET, SO_ERROR, error_code, &error_code_size) == NET_ERROR)
	{
#ifdef _WIN32
		if ((status = WSAGetLastError()) == WSAENOTSOCK)
			Log::logSuccessMsg("Socket already closed or never initialized");
		else
			Log::logFailureMsg("Error when closing socket: " + status);
#elif __linux__ || __unix__ || __unix || unix || __APPLE__ || __MACH__
		LogNetwork::logFailureMsg("Error when closing socket");
#endif
	}
	else
	{
		LogNetwork::logSuccessMsg("Successfully close socket");
		__close_socket(_sock);
	}
	_connected = false;
#ifdef __WIN32
	stopWSA();
#endif
}
		
ERR ClientNetwork::readData(std::string &data)
{
	__ret ret;
	char buff[(SIZE_BUFF + sizeof(char))] = { 0 };

	while ((ret = __read_socket(_sock, buff, SIZE_BUFF, 0)) == SIZE_BUFF)
	{
		data.append(buff, SIZE_BUFF);
		memset(buff, 0, (SIZE_BUFF + sizeof(char)));
	}
	if (ret > 0)
	{
		buff[ret] = '\0'; // to be sure
		data.append(buff, static_cast<unsigned long>(ret));
	}
	else if (ret < 0)
	{
		LogNetwork::logFailureMsg("Error failed to read data from socket");
		return (NET_ERROR);
	}
	else
	{
		LogNetwork::logInfoMsg("Received deconnection notification from server");
		deconnectToServer();
		return (SUCCESS);
	}
	LogNetwork::logSomething("\nReceived:\n" + data + "\n:From Server\n");
	return (SUCCESS);
}

ERR ClientNetwork::writeData(const std::string &data)
{
	auto size = (__size)data.size();
	const char *dataToSend = data.c_str();

	if ((__write_socket(_sock, dataToSend, size, 0)) == NET_ERROR)
	{
		LogNetwork::logFailureMsg("\nError failed to write data to socket");
		return (NET_ERROR);
	}
	LogNetwork::logSomething("\nSend:\n" + data + "\n:To server\n");
	return (SUCCESS);
}

bool ClientNetwork::isDataToRead()
{
	clearSocket();
	if (select(((__socket)_sock + 1), &_fd_set, nullptr, nullptr, nullptr) == -1)
	{
		logFailureMsg("Error on select ", true);
		return (false);
	}
	return (FD_ISSET(_sock, &_fd_set));
}

bool ClientNetwork::isConnected() const
{
	return (_connected);
}

void ClientNetwork::clearSocket()
{
	FD_ZERO(&_fd_set);
	FD_SET(_sock, &_fd_set);
	LogNetwork::logSomething("Socket: " + std::to_string(_sock) + " cleared\n");
}

void ClientNetwork::logFailureMsg(const std::string &msg, __attribute__((unused))bool errorCode)
{
#ifdef _WIN32
	if (errorCode)
		LogNetwork::logFailureMsg(msg + WSAGetLastError());
	else
		LogNetwork::logFailureMsg(msg);
#elif __linux__ || __unix__ || __unix || unix || __APPLE__ || __MACH__
	LogNetwork::logFailureMsg(msg);
#endif
}

void ClientNetwork::setLogActive(bool log)
{
    _logActive = log;
    LogNetwork::setLogActive(log);
}

bool ClientNetwork::isLogActive() const
{
    return (_logActive);
}