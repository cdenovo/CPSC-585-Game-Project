#pragma once

#include <iostream>
#include <Windows.h>
#include <sstream>
#include "Intention.h"
#include "NetworkInfo.h"

#define MAXCLIENTS 7
#define NETWORK_PORT 5869

class Server
{
public:
	Server();
	~Server();

	bool gameStarted;
	std::string track;

	int lobbyListen();
	int raceListen();
	
	int setupWSA();
	int setupTCPSocket();
	int setupUDPSocket();

	int changeTrack(std::string track);
	int startGame();
	int endGame();
	void getTCPMessages();
	void getUDPMessages();
	int sendLobbyInfo();
	int sendTCPMessage(const char* message, int length, int clientID);
	int sendUDPMessage(std::string message, int clientID);
	int sendID(int);
	
	int update(std::string worldState);

private:

	void closeConnection();

	SOCKET sTCP; //Handle for the tcp socket
	SOCKET sUDP; //for the udp socket
	WSADATA w; //holds winsock info

	
	ClientInfo clients[MAXCLIENTS];
	int numClients;

	bool wsa_ready;
	bool tcp_ready;
	bool udp_ready;
	char in_buffer[100];
	Intention intents[MAXCLIENTS];
};

