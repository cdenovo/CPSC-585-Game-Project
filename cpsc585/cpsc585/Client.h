#pragma once

#include <winsock.h>
#include <iostream>

#include "Intention.h"

class Client
{
public:
	Client();
	~Client();

	bool connectToServer(int port, std::string ipAddress);

	int ready();

	int sendButtonState(Intention intention);

private:
	int sendTCPMessage(std::string message);
	int sendUDPMessage(std::string message);
	void closeConnection();

	SOCKET sTCP;
	SOCKET sUDP;
	SOCKADDR_IN target; //Socket address info
};

