
#include "ClientNetwork_Windows_SSL.hh"

ClientNetwork_Windows_SSL::ClientNetwork_Windows_SSL() = default;

ClientNetwork_Windows_SSL::~ClientNetwork_Windows_SSL()
{
	deconnectToServer();
}

ERR ClientNetwork_Windows_SSL::initNetworkClient(const t_serverParam & srvParam)
{
	if (AClientNetwork_Windows::initNetworkClient(srvParam) == NET_ERROR)
		return (NET_ERROR);
	std::cout << std::endl << "Launch SSL initialization of the client..." << std::endl << std::endl;
	initSSLLibrary();
	if (initSSLConnection() == SSL_NET_ERROR)
		return (SSL_NET_ERROR);
	if (attachSSLToSocket() == SSL_NET_ERROR)
		return (SSL_NET_ERROR);
	return (SUCCESS);
}

ERR ClientNetwork_Windows_SSL::connectToServer()
{
	if (AClientNetwork_Windows::connectToServer() == NET_ERROR)
		return (NET_ERROR);
	std::cout << "Trying to connect SSL to the server" << std::endl;
	if (connectSSLToServer() == SSL_NET_ERROR)
		return (SSL_NET_ERROR);
	return (SUCCESS);
}

void ClientNetwork_Windows_SSL::deconnectToServer()
{
	std::cout << std::endl << "Cleaning SSL server connection..." << std::endl;
	SSL_shutdown(_ssl);
	SSL_free(_ssl);
	std::cout << "Successfully shutdown SSL connection" << std::endl;
}

ERR ClientNetwork_Windows_SSL::readData(std::string & data)
{
	int ret;
	char buff[(SIZE_BUFF + sizeof(char))] = { 0 };

	if ((ret = SSL_read(_ssl, buff, SIZE_BUFF)) < 0)
	{
		std::cerr << "Error failed to read data from SSL socket with error code: " << ret << std::endl;
		return (SSL_NET_ERROR);
	}
	buff[ret] = '\0'; // to be sure
	data = std::string(buff);
	std::cout << "Received: " << std::endl << data << std::endl << ":From Server" << std::endl << std::endl;
	return (SUCCESS);
}

ERR ClientNetwork_Windows_SSL::writeData(const std::string & data)
{
	int status;
	int size = (int)(data.size() + strlen(_srvParam.endCharSrv));
	const char *dataToSend = (data + _srvParam.endCharSrv).c_str();

	if ((status = SSL_write(_ssl, dataToSend, size)) < 0)
	{
		std::cerr << "Error failed to write data to SSL socket with error code: " << status << std::endl;
		return (NET_ERROR);
	}
	std::cout << "Send:" << std::endl << data << std::endl << ":To server" << std::endl << std::endl;
	return (SUCCESS);
}

void ClientNetwork_Windows_SSL::initSSLLibrary()
{
	SSL_load_error_strings();
	SSL_library_init();
	std::cout << "Successfully initialize SSL library" << std::endl;
}

ERR ClientNetwork_Windows_SSL::initSSLConnection()
{
	if ((_ssl_meth = TLSv1_2_client_method()) == NULL)
	{
		std::cerr << "Error failed to create SSL METHOD object" << std::endl;
		return (SSL_NET_ERROR);
	}
	if ((_ssl_ctx = SSL_CTX_new(_ssl_meth)) == NULL)
	{
		std::cerr << "Error failed to create SSL CTX object - Please check SLL Method protocol" << std::endl;
		return (SSL_NET_ERROR);
	}
	if ((_ssl = SSL_new(_ssl_ctx)) == NULL)
	{
		std::cerr << "Error failed to create SSL object - Please check SLL Method protocol" << std::endl;
		return (SSL_NET_ERROR);
	}
	std::cout << "Successfully initialize SSL connection" << std::endl;
	return (SUCCESS);
}

ERR ClientNetwork_Windows_SSL::attachSSLToSocket()
{
	SSL_set_fd(_ssl, (int)_sock);
	std::cout << "Successfully attach SSL connection to existing socket" << std::endl;
	return (SUCCESS);
}

ERR ClientNetwork_Windows_SSL::connectSSLToServer()
{
	if (SSL_connect(_ssl) <= 0)
	{
		std::cerr << "Error during SSL connection to server" << std::endl;
		return (SSL_NET_ERROR);
	}
	std::cout << "Successfully connect SSL to server using cipher: " << SSL_get_cipher(_ssl) << std::endl << std::endl;
	return (SUCCESS);
}
