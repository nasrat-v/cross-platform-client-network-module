
#ifndef __HTTPSCONNECTION__
#define __HTTPSCONNECTION__

#include "../../src/ClientNetwork_SSL.hh"

# define PORT			443
# define HOSTNAME		"api.telegram.org"
# define END_CHAR_SRV	"\r\n\r\n"
# define BOT_TOKEN		"/bot735059832:AAFIjSGNKHnDL6K5eXUDLDMznd92h4sMQ6E"
# define BOT_METHOD		"/sendMessage"
# define GROUP_CHAT_ID	"-291696948"
# define PARSE_MODE		"html"

class HttpsConnection
{
public:
	HttpsConnection();
	~HttpsConnection();

	ERR					initHttpsClient();
	ERR					communicateWithHttpsServer(const std::string &message);

private:
	/* Attributes */
	ClientNetwork_SSL	_network;

	/* Methods */
	const ClientNetwork_SSL::t_serverParam	initConfigurationHttpsServer();
};

#endif /* !__HTTPSCONNECTION__ */