#pragma once

#include "IClientNetwork.hh"

class AClientNetwork_Linux : public IClientNetwork
{
public:
	AClientNetwork_Linux();
	virtual ~AClientNetwork_Linux();

	ERR	initNetworkClient(const t_serverParam &srvParam);
	ERR	connectToServer();
	void	deconnectToServer();
	ERR	readData(std::string &data);
	ERR	writeData(const std::string &data);
	bool	isDataToRead();
	void	clearSocket();

protected:
	ERR	initSocket();
	ERR	initHandleSocketWithIpAddress();
	void	initHandleSocketWithHostname();
	ERR	findIpAddrWithHostname();
};

