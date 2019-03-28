
#ifndef __CLIENTNETWORK_SSL__
#define __CLIENTNETWORK_SSL__

#include <openssl/ssl.h>

#include "ClientNetwork.hh"

#if (OPENSSL_VERSION_NUMBER < 0x10100000L)
    # define CLIENT_METHOD  TLSv1_2_client_method
#else
    # define CLIENT_METHOD  TLS_client_method
#endif

# define SSL_NET_ERROR      -2

class ClientNetwork_SSL : public ClientNetwork
{
public:
    ClientNetwork_SSL();
    ~ClientNetwork_SSL() override;

    ERR         initNetworkClient(const t_serverParam &srvParam) override; // override -> add init SSL
    ERR         connectToServer() override; // override -> add connect SSL
    void        deconnectToServer() override; // override -> add SLL_shutdown & SSL_free
    ERR         readData(std::string &data) override; // override -> just change recv by SSL_read
    ERR         writeData(const std::string &data) override; // override -> just change send by SSL_write

private:
    /* Attributes */
    const SSL_METHOD    *_ssl_meth;
    SSL_CTX             *_ssl_ctx;
    SSL                 *_ssl;

    /* Methods */
    void        initSSLLibrary();
    ERR         initSSLConnection();
    ERR         attachSSLToSocket();
    ERR         connectSSLToServer();
};

#endif /* !__CLIENTNETWORK_SSL__ */
