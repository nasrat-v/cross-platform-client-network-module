
#include "HttpsConnection.hh"

HttpsConnection::HttpsConnection() = default;

HttpsConnection::~HttpsConnection() = default;

ERR HttpsConnection::initHttpsClient()
{
	int status;
	ClientNetwork_SSL::t_serverParam srvParam;

	srvParam = initConfigurationHttpsServer();
	if (((status = _network.initNetworkClient(srvParam)) == NET_ERROR) || (status == SSL_NET_ERROR))
	{
		Log::logFailureMsg("Config error with https server at " + 
							std::string(srvParam.hostName) + ":" + std::to_string(srvParam.port));
		return (NET_ERROR);
	}
	return (SUCCESS);
}

const ClientNetwork_SSL::t_serverParam HttpsConnection::initConfigurationHttpsServer()
{
	ClientNetwork_SSL::t_serverParam srvParam;

	memset(&srvParam, 0, sizeof(srvParam));
	srvParam.ipType = AF_INET;
	srvParam.socketType = SOCK_STREAM;
	srvParam.protocol = IPPROTO_TCP;
	srvParam.hostName = HOSTNAME;
	srvParam.serviceName = nullptr;
	srvParam.ipAddr = nullptr;
	srvParam.port = PORT;
	return (srvParam);
}

ERR HttpsConnection::communicateWithHttpsServer(const std::string &message)
{
	std::string data;

	if (_network.connectToServer() == NET_ERROR)
		return (NET_ERROR);
	if (_network.writeData(std::string("GET ") + BOT_TOKEN + BOT_METHOD + "?chat_id=" + GROUP_CHAT_ID + 
							"&text=" + message + "&parse_mode=" + PARSE_MODE + END_CHAR_SRV) == NET_ERROR)
		return (NET_ERROR);
	while (_network.isConnected())
	{
		if (_network.isDataToRead())
		{
			_network.readData(data);
			resetString(data);
		}
	}
	return (SUCCESS);
}

void HttpsConnection::resetString(std::string &str)
{
	str = "";
}