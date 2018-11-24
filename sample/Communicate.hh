#pragma once

#include "WinNetwork.hh"

# define PORT		6667
# define HOSTNAME	"chat.freenode.net"
# define NICKNAME	"nick toto"
# define USERNAME	"user toto  8 * :toto"
# define END_CHAR_SRV	"\r\n"

class Communicate
{
public:
	Communicate();
	~Communicate();

	void		communicateWithIRCServer();
	ERR		initIRCClient();

private:
	/* Attributes */
	bool		_isInit;
	WinNetwork	_network;

	/* Methods */
	const WinNetwork::t_serverParam	initConfigurationIRCServer();
};
