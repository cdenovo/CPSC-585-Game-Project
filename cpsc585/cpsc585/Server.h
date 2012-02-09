#pragma once

#include <winsock.h>
#include <iostream>
#include <Windows.h>
#include <sstream>

#define MAXCLIENTS 7

class Server
{
public:
	Server();
	~Server();

	int listenOnPort(int port);

	int playerJoined(int ID);
	int changeTrack(std::string track);
	int startGame(std::string track, int pos[]);
	int endGame();
	
	int update(std::string worldState);

private:
	void closeConnection();
	int sendTCPMessage(const char* message, int length, int clientID);
	int sendTCPMessage(const std::string &message, int clientID);
	int sendUDPMessage(std::string message, int clientID);

	SOCKET sTCP; //Handle for the socket
	SOCKET sUDP;
	WSADATA w;
	SOCKADDR_IN clients[MAXCLIENTS];
	SOCKET clientSocks[MAXCLIENTS];
	int numClients;
};

