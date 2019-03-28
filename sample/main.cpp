//
// Created by nasrat_v on 3/3/19.
//

#include "ssl_sample/HttpsConnection.hh"

int main()
{
    HttpsConnection conn;

    if (conn.initHttpsClient() == NET_ERROR)
        return (1);
    if (conn.communicateWithHttpsServer("salut la famille") == NET_ERROR)
        return (1);
    return (0);
}