#pragma once

#include "ClientNetwork_Windows_SSL.hh"

# define PORT			443
# define HOSTNAME		"api.telegram.org"
# define END_CHAR_SRV	"\r\n\r\n"
# define BOT_TOKEN		"/bot735059832:AAFIjSGNKHnDL6K5eXUDLDMznd92h4sMQ6E"	
# define BOT_ID			"735059832"
# define BOT_METHOD		"/sendMessage"
# define GROUP_CHAT_ID	"-291696948"
# define PARSE_MODE		"html"

class HttpsCommunication
{
public:
	HttpsCommunication();
	~HttpsCommunication();

	ERR			communicate(const std::string &message);
	ERR			initHttpsClient();

private:
	/* Attributes */
	ClientNetwork_Windows_SSL	_network;

	/* Methods */
	const ClientNetwork_Windows_SSL::t_serverParam	initConfigurationHttpsServer();
};