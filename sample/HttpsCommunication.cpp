
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

void HttpsCommunication::communicate()
{
	std::string data;

	_network.connectToServer();
	_network.writeData("GET / HTTP/1.1\r\nHost: www.google.com");
	//_network.writeData("GET /bot735059832:AAFIjSGNKHnDL6K5eXUDLDMznd92h4sMQ6E/getMe HTTP/1.1\r\nHost: api.telegram.org");
	if (_network.isDataToRead())
		_network.readData(data);
}

int main()
{
	HttpsCommunication httpsCom;

	httpsCom.initHttpsClient();
	httpsCom.communicate();
	return (0);
}

