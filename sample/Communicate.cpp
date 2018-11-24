/*
* Example of communication with an IRC Server using WinNetwork class
* 24/11/2018
* nasrat_v
*/

#include "Communicate.hh"

Communicate::Communicate()
{
	_isInit = false;
}

Communicate::~Communicate() = default;

ERR Communicate::initIRCClient()
{
	WinNetwork::t_serverParam srvParam;

	srvParam = initConfigurationIRCServer();
	if (!_network.initNetworkClient(srvParam))
	{
		std::cerr << "Config error with irc server at " << srvParam.hostName << ':' << srvParam.port << std::endl;
		return (NET_ERROR);
	}
	_isInit = true;
	return (SUCCESS);
}

const WinNetwork::t_serverParam Communicate::initConfigurationIRCServer()
{
	WinNetwork::t_serverParam srvParam;

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

void Communicate::communicateWithIRCServer()
{
	bool end = false;
	std::string data;

	if (_isInit)
	{
		if (_network.connectToServer() != NET_ERROR)
		{
			_network.writeData(NICKNAME);
			_network.writeData(USERNAME);
			while (!end)
			{
				_network.clearSocket();
				if (_network.isDataToRead())
				{
					if (_network.readData(data) != NET_ERROR)
						std::cout << data << std::endl;
				}
			}
		}
	}
}

int main()
{
	Communicate com;

	com.initIRCClient();
	com.communicateWithIRCServer();
	return (0);
}

