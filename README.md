# Socket-Programming-TCP-UDP-
Basic socket programming project with 2 pairs of server/client one built on TCP and the other on UDP

Guessing Game built on TCP/UDP servers/clients
Isaiah Balthazar

Purpose: 

This project was created to explore the relationships of a server/client pair and how their interactions are different when built on TCP(connection-oriented) vs UDP(connectionless) sockets. Each server/client pair performs the same guessing game. In which the server randomly selects an integer from 1-100. And the client sends their name, original guess, and afterwards their repeated guesses. After each guess the server responds with “High”, “Low” or “Correct”. The main difference between the different protocol implementations is for the TCP version the client connects to the server and then the game is started whereas for the UDP version no handshake is needed and the back and forth responses can just begin.

To compile:

 Use makefile and type: “make”

To run:

Run server in background using: “./tcpserver &” or “./udpserver &”. This uses a default port number 1212. If you want to use a different port number you run using “./tcpserver [port #] &” or “./udpserver [port #] &”

Run client using: “./tcpclient” or “./udpclient”. This also uses a default port number 1212. If you want to use a different port number you need to specify the host as well and use “./tcpclient [host [port #]]” or “./udpclient [host [port #]]”. If you want to use the default port number but a different host you just use “./tcpclient [host]” or “./udpclient [host]”.

