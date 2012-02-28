#pragma once

#include <iostream>
#include <sstream>
#include <string.h>

#include "Intention.h"
#include "NetworkInfo.h"

class Client
{
public:
	Client();
	~Client();

	bool connectToServer(int port, std::string ipAddress);
	void getTCPMessages();
	void getUDPMessages();

	int ready();
	int unready();
	int setColor(int color);

	int sendButtonState(Intention intention);

	std::string world;
	std::string track;
	bool isReady;
	bool start;
	int id;
	bool end;

private:
	int sendTCPMessage(std::string message);
	int sendTCPMessage(const char* message, int length);
	int sendUDPMessage(const char* message, int length);
	void closeConnection();

	SOCKET sTCP;
	SOCKET sUDP;
	SOCKADDR_IN target; //Socket address info

	ClientInfo clients[MAXCLIENTS];
	int numClients;
};

