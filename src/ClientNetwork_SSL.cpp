
#include "ClientNetwork_SSL.hh"

ClientNetwork_SSL::ClientNetwork_SSL() = default;

ClientNetwork_SSL::~ClientNetwork_SSL() = default;

ERR ClientNetwork_SSL::initNetworkClient(const t_serverParam & srvParam)
{
	if (ClientNetwork::initNetworkClient(srvParam) == NET_ERROR)
		return (NET_ERROR);
	Log::logInfoMsg("Launch SSL initialization of the client...");
	initSSLLibrary();
	if (initSSLConnection() == SSL_NET_ERROR)
		return (SSL_NET_ERROR);
	if (attachSSLToSocket() == SSL_NET_ERROR)
		return (SSL_NET_ERROR);
	return (SUCCESS);
}

ERR ClientNetwork_SSL::connectToServer()
{
	if (ClientNetwork::connectToServer() == NET_ERROR)
		return (NET_ERROR);
	Log::logInfoMsg("Trying to connect SSL to the server");
	if (connectSSLToServer() == SSL_NET_ERROR)
		return (SSL_NET_ERROR);
	return (SUCCESS);
}

void ClientNetwork_SSL::deconnectToServer()
{
	Log::logInfoMsg("Cleaning SSL server connection...");
	SSL_shutdown(_ssl);
	SSL_free(_ssl);
	Log::logSuccessMsg("Successfully shutdown SSL connection");
	ClientNetwork::deconnectToServer();
}

ERR ClientNetwork_SSL::readData(std::string & data)
{
	__ret ret;
	char buff[(SIZE_BUFF + sizeof(char))] = { 0 };

	while ((ret = SSL_read(_ssl, buff, SIZE_BUFF)) == SIZE_BUFF)
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
		Log::logFailureMsg("Error failed to read data from SSL socket. Code: " + std::to_string(ret));
		return (SSL_NET_ERROR);
	}
	else
	{
		Log::logInfoMsg("Received deconnection notification from server");
		deconnectToServer();
		return (SUCCESS);
	}
	Log::logSomething("\nReceived:\n" + data + "\n:From Server\n");
	return (SUCCESS);
}

ERR ClientNetwork_SSL::writeData(const std::string & data)
{
	__ret ret;
	auto size = (int)data.size();
	const char *dataToSend = data.c_str();

	if ((ret = SSL_write(_ssl, dataToSend, size)) < 0)
	{
		Log::logFailureMsg("Error failed to write data from SSL socket. Code: " + std::to_string(ret));
		return (SSL_NET_ERROR);
	}
	Log::logSomething("\nSend:\n" + data + "\n:To server\n");
	return (SUCCESS);
}

void ClientNetwork_SSL::initSSLLibrary()
{
	SSL_load_error_strings();
	SSL_library_init();
	Log::logSuccessMsg("Successfully initialize SSL library");
}

ERR ClientNetwork_SSL::initSSLConnection()
{
	if ((_ssl_meth = CLIENT_METHOD()) == nullptr)
	{
		Log::logFailureMsg("Error failed to create SSL METHOD object");
		return (SSL_NET_ERROR);
	}
	if ((_ssl_ctx = SSL_CTX_new(_ssl_meth)) == nullptr)
	{
		Log::logFailureMsg("Error failed to create SSL CTX object - Check SLL Method protocol");
		return (SSL_NET_ERROR);
	}
	if ((_ssl = SSL_new(_ssl_ctx)) == nullptr)
	{
		Log::logFailureMsg("Error failed to create SSL object - Check SLL Method protocol");
		return (SSL_NET_ERROR);
	}
	Log::logSuccessMsg("Successfully initialize SSL connection");
	return (SUCCESS);
}

ERR ClientNetwork_SSL::attachSSLToSocket()
{
	SSL_set_fd(_ssl, (int)_sock);
	Log::logSuccessMsg("Successfully attach SSL connection to existing socket");
	return (SUCCESS);
}

ERR ClientNetwork_SSL::connectSSLToServer()
{
	if (SSL_connect(_ssl) <= 0)
	{
		Log::logFailureMsg("Error during SSL connection to server");
		return (SSL_NET_ERROR);
	}
	Log::logSuccessMsg("Successfully connect SSL to server using cipher: " + std::string(SSL_get_cipher(_ssl)));
	return (SUCCESS);
}
