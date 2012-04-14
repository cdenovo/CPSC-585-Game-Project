#pragma once

#include <iostream>
#include <sstream>
#include <string.h>

#include "Intention.h"
#include "NetworkInfo.h"
#include "Racer.h"

#define BULLY_TIMEOUT 3.0

class Client
{
public:
	Client();
	~Client();

	bool connectToServer(int port, std::string ipAddress);
	void getTCPMessages(float milliseconds);
	int getUDPMessages(float milliseconds);

	int setupWSA();
	int setupUDPSocket();

	int ready();
	int unready();
	int setColor(int color);

	int sendAliveMessage();
	int sendElectionMessage();
	int sendButtonState(Intention &intention);
	int sendLeaderMessage();
	int sendBullyMessage(int);

	std::string track;
	bool isReady;
	bool start;
	int id;
	bool end;
	bool newWorldInfo;
	int numClients;
	int serverID;
	ClientInfo clients[MAXCLIENTS];
	RacerData world[MAXCLIENTS];
	DynamicObjManager* dynManager;


private:
	int sendTCPMessage(std::string message);
	int sendTCPMessage(const char* message, int length);
	int sendUDPMessage(const char* message, int length);
	int sendUDPMessageTo(int,char*,int);
	void closeConnection();

	SOCKET sTCP;
	SOCKET sUDP;
	SOCKADDR_IN target; //Socket address info	
	bool udp_ready;
	bool wsa_ready;
	WSADATA w; //holds winsock info
	int seqNum;

	int election_winner;
	bool is_electing;
	bool was_bullied;
	bool to_be_leader;

	double bullied_no_leader_timer;
	double leader_nack_timer;
	double no_response_timer;


};

