//
// Created by nasrat_v on 3/3/19.
//

#ifndef __CONNECTION__
#define __CONNECTION__

# define IPADDR     "127.0.0.1"
# define PORT       4343

#include "../src/ClientNetwork.hh"

class Connection
{
public:
    Connection();
    ~Connection();

    ERR             initClient(bool log);
    ERR             communicateWithServer(const std::string &msg);

private:
    /* Attributes */
    ClientNetwork   _network;

    /* Methods */
    const ClientNetwork::t_serverParam  initConfigurationServer();
    void 							    resetString(std::string &str);
};


#endif /* !__CONNECTION__ */
