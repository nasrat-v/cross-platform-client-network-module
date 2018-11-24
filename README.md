# windows-socket-class
A network class with win32 socket to implement in all client network project

## Src
The source folder contain the WinNetwork class.
This class can be used to initialize only one client.
But you can connect and deconnect from multiple server with the same object.
A structure "t_serverParam" allows you to send all configuration needed for the communication with the server.

### Ip address and Hostname
If the Ip Address of the server is unknwown, you can use a hostname.

### Sample
This repository present a sample folder with an exemple of how using the WinNetwork class with an IRC Server.
