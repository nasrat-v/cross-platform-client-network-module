#pragma once

#include <openssl/ssl.h>

#include "AClientNetwork_Windows.hh"

# define SSL_NET_ERROR	-2

class ClientNetwork_Windows_SSL : public AClientNetwork_Windows
{
public:
	ClientNetwork_Windows_SSL();
	~ClientNetwork_Windows_SSL();

	ERR			initNetworkClient(const t_serverParam &srvParam);
	ERR			connectToServer();
	void			deconnectToServer();
	ERR			readData(std::string &data);
	ERR			writeData(const std::string &data);

private:
	/* Attributes */
	const SSL_METHOD	*_ssl_meth;
	SSL_CTX			*_ssl_ctx;
	SSL			*_ssl;

	/* Methods */
	void			initSSLLibrary();
	ERR			initSSLConnection();
	ERR			attachSSLToSocket();
	ERR			connectSSLToServer();
};

