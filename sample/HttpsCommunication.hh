#pragma once

#include "ClientNetwork_Windows_SSL.hh"

# define PORT				443
# define HOSTNAME			"api.telegram.org"
# define END_CHAR_SRV		"\r\n\r\n"

class HttpsCommunication
{
public:
	HttpsCommunication();
	~HttpsCommunication();

	void		communicate();
	ERR			initHttpsClient();

private:
	/* Attributes */
	ClientNetwork_Windows_SSL	_network;

	/* Methods */
	const ClientNetwork_Windows_SSL::t_serverParam	initConfigurationHttpsServer();
};