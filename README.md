# TCP_Client-Server

Run tcpserver with './tcpserver [port_number]'

and 

tcpclient or tcpconcurrent_client with './tcpclient(_concurrency) [port_number] [host] [message]'

**************************************************************************************************

With [host] is 'localhost' if running on the same machine as the server or the IP address of the server otherwise, [port_number] is the same for both server and client, and [message] is any string. This string will be capitalised by the server and sent back to the client.
