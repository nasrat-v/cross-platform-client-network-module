//
// Created by nasrat_v on 3/3/19.
//

#include "Communication.hh"

Connection::Connection() = default;

Connection::~Connection() = default;

ERR Connection::initHttpsClient()
{
    ClientNetwork::t_serverParam srvParam;

    srvParam = initConfigurationServer();
    if (_network.initNetworkClient(srvParam) == NET_ERROR)
    {
        Log::logFailureMsg("Config error with server at " +
                           std::string(srvParam.hostName) + ":" + std::to_string(srvParam.port));
        return (NET_ERROR);
    }
    return (SUCCESS);
}

const ClientNetwork::t_serverParam Connection::initConfigurationServer()
{
    ClientNetwork::t_serverParam srvParam;

    memset(&srvParam, 0, sizeof(srvParam));
    srvParam.ipType = AF_INET;
    srvParam.socketType = SOCK_STREAM;
    srvParam.protocol = IPPROTO_TCP;
    srvParam.hostName = nullptr;
    srvParam.serviceName = nullptr;
    srvParam.ipAddr = IPADDR;
    srvParam.port = PORT;
    return (srvParam);
}

ERR Connection::communicateWithServer(const std::string &msg)
{
    std::string data;

    if (_network.connectToServer() == NET_ERROR)
        return (NET_ERROR);
    if (_network.writeData(msg) == NET_ERROR)
        return (NET_ERROR);
    while (_network.isConnected())
    {
        if (_network.isDataToRead())
            _network.readData(data);
    }
    return (SUCCESS);
}