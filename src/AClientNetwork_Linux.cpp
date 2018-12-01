
#include "AClientNetwork_Linux.hh"

AClientNetwork_Linux::AClientNetwork_Linux()
{
}

AClientNetwork_Linux::~AClientNetwork_Linux()
{
}

ERR AClientNetwork_Linux::initNetworkClient(const t_serverParam & srvParam)
{
	return ERR();
}

ERR AClientNetwork_Linux::connectToServer()
{
	return ERR();
}

void AClientNetwork_Linux::deconnectToServer()
{
}

ERR AClientNetwork_Linux::readData(std::string & data)
{
	return ERR();
}

ERR AClientNetwork_Linux::writeData(const std::string & data)
{
	return ERR();
}

bool AClientNetwork_Linux::isDataToRead()
{
	return false;
}

void AClientNetwork_Linux::clearSocket()
{
}

ERR AClientNetwork_Linux::initSocket()
{
	return ERR();
}

ERR AClientNetwork_Linux::initHandleSocketWithIpAddress()
{
	return ERR();
}

void AClientNetwork_Linux::initHandleSocketWithHostname()
{
}

ERR AClientNetwork_Linux::findIpAddrWithHostname()
{
	return ERR();
}
