#pragma once

#include <WinSock.h>
//#include <inaddr.h>

const int MAXCLIENTS = 5;
const int NETWORK_PORT = 5869;

//Client headers
const char BUTTON = 'B';
const char READY = 'R';
const char UNREADY = 'U';
const char COLOR = 'C';
const char ALIVE = 'A';

//Server headers
const char TRACK = 'T';
const char START = 'S';
const char END = 'E';
const char CLIENTINFO = 'L';
const char WORLDSTATE = 'W';
const char ID = 'I';

struct ClientInfo
{
	int id;
	int color;
	SOCKET sock;
	bool ready;
	bool connected;
	float millisecond_lag;
	SOCKADDR_IN addr;
};