
#include "HttpsCommunication.hh"

HttpsCommunication::HttpsCommunication() = default;

HttpsCommunication::~HttpsCommunication() = default;

ERR HttpsCommunication::initHttpsClient()
{
	ClientNetwork_Windows_SSL::t_serverParam srvParam;

	srvParam = initConfigurationHttpsServer();
	if (!_network.initNetworkClient(srvParam))
	{
		std::cerr << "Config error with https server at " << srvParam.hostName << ':' << srvParam.port << std::endl;
		return (NET_ERROR);
	}
	return (SUCCESS);
}

const ClientNetwork_Windows_SSL::t_serverParam HttpsCommunication::initConfigurationHttpsServer()
{
	ClientNetwork_Windows_SSL::t_serverParam srvParam;

	memset(&srvParam, 0, sizeof(srvParam));
	srvParam.ipType = AF_INET;
	srvParam.socketType = SOCK_STREAM;
	srvParam.protocol = IPPROTO_TCP;
	srvParam.hostName = HOSTNAME;
	srvParam.serviceName = NULL;
	srvParam.ipAddr = NULL;
	srvParam.port = PORT;
	srvParam.endCharSrv = END_CHAR_SRV;
	return (srvParam);
}

ERR HttpsCommunication::communicate()
{
	std::string data;

	if (_network.connectToServer() == NET_ERROR)
		return (NET_ERROR);
	if (_network.writeData("GET / HTTP/1.1\r\nHost: " + std::string(HOSTNAME)) == NET_ERROR)
		return (NET_ERROR);
	while (42)
	{
		if (_network.isDataToRead())
			_network.readData(data);
	}
}

int main()
{
	HttpsCommunication httpsCom;

	if (httpsCom.initHttpsClient() == NET_ERROR)
		return (1);
	if (httpsCom.communicate() == NET_ERROR)
		return (1);
	return (0);
}

