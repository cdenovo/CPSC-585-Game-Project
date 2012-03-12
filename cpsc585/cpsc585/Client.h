#pragma once

#include <iostream>
#include <sstream>
#include <string.h>

#include "Intention.h"
#include "NetworkInfo.h"
#include "Racer.h"

class Client
{
public:
	Client();
	~Client();

	bool connectToServer(int port, std::string ipAddress);
	void getTCPMessages();
	void getUDPMessages();

	int setupWSA();
	int setupUDPSocket();

	int ready();
	int unready();
	int setColor(int color);

	int sendButtonState(Intention intention);

	std::string track;
	bool isReady;
	bool start;
	int id;
	bool end;
	bool newWorldInfo;
	int numClients;
	ClientInfo clients[MAXCLIENTS];
	char world[MAXCLIENTS][RACERSIZE];


private:
	int sendTCPMessage(std::string message);
	int sendTCPMessage(const char* message, int length);
	int sendUDPMessage(const char* message, int length);
	void closeConnection();

	SOCKET sTCP;
	SOCKET sUDP;
	SOCKADDR_IN target; //Socket address info	
	bool udp_ready;
	bool wsa_ready;
	WSADATA w; //holds winsock info
};

