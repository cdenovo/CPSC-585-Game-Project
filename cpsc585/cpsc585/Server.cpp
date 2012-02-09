#include "Server.h"

Server::Server()
{
	numClients = 0;
}

Server::~Server()
{
}

/**
 * Listens on the given port for incomming connections
 * From: http://www.codeproject.com/Articles/13071/Programming-Windows-TCP-Sockets-in-C-for-the-Begin
 */
int Server::listenOnPort(int port)
{
	bool success = false;
	int error = WSAStartup(0x0202, &w); //Fill in WSA info

	if(!error)
	{
		if(w.wVersion != 0x0202) //Wrong winsock version?
		{
			WSACleanup();
		}
		else //If right winsock version
		{
			SOCKADDR_IN addr; //The address struct for TCP socket

			addr.sin_family = AF_INET; //Set address family
			addr.sin_port = htons(port); //Assign port to socket

			addr.sin_addr.s_addr = htonl(INADDR_ANY); //Accept connection from any IP

			sTCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Create socket

			if(sTCP != INVALID_SOCKET)
			{
				if(bind(sTCP, (LPSOCKADDR)&addr, sizeof(addr)) != SOCKET_ERROR)
				{
					listen(sTCP, SOMAXCONN); //Allows max # of connections as possible, start listening
					int size = sizeof(clients[numClients]); //Get size of addr struct
					clientSocks[numClients] = accept(sTCP, (LPSOCKADDR)&clients[numClients], &size); //Accept connection and store data in struct
					numClients++; //Increment the number of clients connected
                    success = true;
					std::cout << "Connection accepted.\n";
				}
			}
		}
	}

	return success;
}

/**
 * Closes the socket and any connection on it
 * From: http://www.codeproject.com/Articles/13071/Programming-Windows-TCP-Sockets-in-C-for-the-Begin
 */
void Server::closeConnection()
{
	//Close the socket if it exists
	if(sTCP)
	{
		closesocket(sTCP);
	}

	WSACleanup(); //Clean up Winsock
}

/**
 * Sends the given message over TCP to the client with the given ID
 */
int Server::sendTCPMessage(const char* message, int length, int clientID)
{
	std::cout << "Client ID: " << clientID << "\n";
	return send(clientSocks[clientID],message,length, 0);
}

int Server::sendTCPMessage(const std::string &message, int clientID)
{
	std::cout << "Client ID: " << clientID << "\n";
	return send(clientSocks[clientID],message.c_str(),message.length(), 0);
}

/**
 * Sends the given message over UDP to the client with the given ID
 */
int Server::sendUDPMessage(std::string message, int clientID)
{
	return sendto(sUDP, message.c_str(), message.length(), 0, (SOCKADDR*) &clients[clientID], sizeof(clients[clientID]));
}

/**
 * Sends message that the player with the given ID has joined the game, sending their id and ip address to all other players,
 * and the player who joined is sent their ID and port.
 */
int Server::playerJoined(int id)
{
	//Send each client a message
	for(int i = 0; i < numClients; i++)
	{
		std::stringstream ss;
		
		if(i == id) //If it is the new player, only send them their player ID
		{
			ss << "ID " << i;
		}
		else //If it is another player, send the ID and IP address and port
		{
			ss << "JOINED " << i << " " << inet_ntoa(clients[id].sin_addr) << " " << clients[id].sin_port;
		}

		sendTCPMessage(ss.str(),i); //Send the message
	}

	return true;
}

/**
 * Server sends to all clients that the track has been changed
 */
int Server::changeTrack(std::string track)
{
	//Get size of message and split into chars
	int size = track.length() + 5;

	char* buffer = new char[size];
	buffer[0] = 'T';
	memcpy(buffer+1,&size,sizeof(int));
	memcpy(buffer+5,track.c_str(),track.length());
	std::cout << "Clients: " << numClients << "\n";

	//Send each client a message
	for(int i = 0; i < numClients; i++)
	{
		int result = sendTCPMessage(buffer,size,i); //Send the message
		if(result == -1)
			result = WSAGetLastError();
		std::cout << "Returned " << result << "\n";
	}

	delete[] buffer;

	return true;
}

/**
 * Server lets all clients know the game is starting, and gives them their starting position
 */
int Server::startGame(std::string track, int startPos[])
{
	//Send each client a message
	for(int i = 0; i < numClients; i++)
	{
		std::stringstream ss;
		
		ss << "START " << track << " " << startPos[i];

		sendTCPMessage(ss.str(),i); //Send the message
	}

	return true;
}

/**
 * Server lets all clients know the game is over and gives the final positions
 */
int Server::endGame(std::string track, int pos[])
{
	//Send each client a message
	for(int i = 0; i < numClients; i++)
	{
		std::stringstream ss;
		
		ss << "END " << pos[i];

		sendTCPMessage(ss.str(),i); //Send the message
	}

	return true;
}

/**
 * Server updates all clients with state of the world
 */
int Server::update(std::string worldState)
{
	//Send each client a message
	for(int i = 0; i < numClients; i++)
	{
		sendTCPMessage("WORLD " + worldState,i); //Send the message
	}

	return true;
}