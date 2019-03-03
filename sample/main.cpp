//
// Created by nasrat_v on 3/3/19.
//

#include "Communication.hh"

int main()
{
    Connection conn;

    if (conn.initHttpsClient() == NET_ERROR)
        return (1);
    if (conn.communicateWithServer("salut la famille") == NET_ERROR)
        return (1);
    return (0);
}