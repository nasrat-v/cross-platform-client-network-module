//
// Created by nasrat_v on 3/3/19.
//

#ifndef CLIENT_NETWORK_MODULE_CONNECTION_H
#define CLIENT_NETWORK_MODULE_CONNECTION_H

# define IPADDR     "127.0.0.1"
# define PORT       4343

#include "../src/ClientNetwork.hh"

class Connection
{
public:
    Connection();
    ~Connection();

    ERR                                 initHttpsClient();
    ERR                                 communicateWithServer(const std::string &msg);

private:
    /* Attributes */
    ClientNetwork                       _network;

    /* Methods */
    const ClientNetwork::t_serverParam  initConfigurationServer();
};


#endif //CLIENT_NETWORK_MODULE_CONNECTION_H
