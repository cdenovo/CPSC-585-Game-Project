#include "Client.h"

Client::Client()
{
}

Client::~Client()
{
}

/**
 * From http://www.codeproject.com/Articles/13071/Programming-Windows-TCP-Sockets-in-C-for-the-Begin
 */
bool Client::connectToServer(int port, std::string ipAddress)
{
	WSADATA wsadata;
	bool success = false;

	int error = WSAStartup(0x0202, &wsadata);

	if(!error)
	{
		//Right Winsock version?
		if(wsadata.wVersion != 0x0202)
		{
			WSACleanup(); //Clean up
		}

		//Set needed information
		target.sin_family = AF_INET; //Address family internet
		target.sin_port = htons(port); //Port to connect on
		target.sin_addr.s_addr = inet_addr(ipAddress.c_str());

		sTCP = socket(AF_INET, SOCK_STREAM, 0); //Create UDP socket
		sUDP = socket(AF_INET, SOCK_DGRAM, 0); // Create UDP socket
		if(sTCP != INVALID_SOCKET) //If we could create socket
		{
			//Try to connect
			if(connect(sTCP, (SOCKADDR*) &target, sizeof(target)) != SOCKET_ERROR)
			{
				success = true;
			}
		}
	}

	return success;
}

int Client::sendTCPMessage(std::string message)
{
	return send(sTCP,message.c_str(),message.length(), 0);
}

int Client::sendUDPMessage(std::string message)
{
	return sendto(sUDP, message.c_str(), message.length(), 0, (SOCKADDR*) &target, sizeof(target));
}

int Client::ready()
{
	return sendTCPMessage("READY");
}

int Client::sendButtonState(Intention intention)
{
	return sendUDPMessage("BUTTON " + intention.serialize());
}

/**
 * Closes the socket and any connection on it
 * From: http://www.codeproject.com/Articles/13071/Programming-Windows-TCP-Sockets-in-C-for-the-Begin
 */
void Client::closeConnection()
{
	//Close the socket if it exists
	if(sTCP)
	{
		closesocket(sTCP);
	}

	WSACleanup(); //Clean up Winsock
}