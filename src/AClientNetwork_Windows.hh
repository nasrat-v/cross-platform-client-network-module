#pragma once

#pragma comment(lib, "ws2_32.lib")

#include "IClientNetwork.hh"

class AClientNetwork_Windows : public IClientNetwork
{
public:
	AClientNetwork_Windows();
	virtual ~AClientNetwork_Windows();

	ERR	initNetworkClient(const t_serverParam &srvParam);
	ERR	connectToServer();
	void	deconnectToServer();
	ERR	readData(std::string &data);
	ERR	writeData(const std::string &data);
	bool	isDataToRead();

protected:
	ERR	initSocket();
	ERR	initHandleSocketWithIpAddress();
	void	initHandleSocketWithHostname();
	ERR	findIpAddrWithHostname();
	void	clearSocket();
};

