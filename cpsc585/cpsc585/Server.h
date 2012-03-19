#pragma once

#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <sstream>
#include "Intention.h"
#include "NetworkInfo.h"
#include "Racer.h"
#include <Windows.h>

class Server
{
public:
	Server();
	~Server();

	bool gameStarted;
	std::string track;

	int lobbyListen(float milliseconds);
	int raceListen(float milliseconds);
	
	int setupWSA();
	int setupTCPSocket();
	int setupUDPSocket();

	int changeTrack(std::string track);
	int startGame();
	int endGame();
	void getTCPMessages(float milliseconds);
	void getUDPMessages();
	int sendLobbyInfo();
	int sendTCPMessage(const char* message, int length, int clientID);
	int sendUDPMessage(const char* message, int length, int clientID);
	int sendUDPMessage(const char* message, int length);
	int sendID(int);
	
	int update(Racer *racers[], int numRacers);

	Intention intents[MAXCLIENTS];
	int numClients;
	ClientInfo clients[MAXCLIENTS];

private:

	void closeConnection();

	SOCKET sTCP; //Handle for the tcp socket
	SOCKET sUDP; //for the udp socket
	WSADATA w; //holds winsock info

	bool wsa_ready;
	bool tcp_ready;
	bool udp_ready;
	char in_buffer[100];
	int seqNum;
};

