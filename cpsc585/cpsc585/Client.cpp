#include "Client.h"

Client::Client()
{
	start = false;
	end = false;
	isReady = false;
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
				std::cout << "Success!\n";
				success = true;
			}
			else
			{
				int error = WSAGetLastError();
				std::cout << "Fail!\n";
			}

			u_long iMode=1;
			ioctlsocket(sTCP,FIONBIO,&iMode);
			//ioctlsocket(sUDP,FIONBIO,&iMode);
		}
	}

	return success;
}

/**
 * Receives any TCP messages in the buffer
 */
void Client::getTCPMessages()
{
	char buff[1000];

	int err = 0;

	while(err != WSAEWOULDBLOCK)
	{
		//Find out the size of the message
		err = recv(sTCP, buff, 5, MSG_PEEK);
		if(err == -1)
		{
			err = WSAGetLastError();
		}

		if(err != WSAEWOULDBLOCK)
		{
			int size = *((int*)(buff+1)); //Get size

			//Get the message
			err = recv(sTCP, buff, size, 0);
			if(err == -1)
			{
				err = WSAGetLastError();
			}

			//Call the correct function depending on the message
			switch(buff[0])
			{
			case TRACK: //Update track
				track = "";

				for(int i = 5; i < size; i++)
				{
					track.append(1, buff[i]);
				}
				break;

			case START: //Start game
				start = true;
				std::cout << "Starting Game!\n";
				break;

			case CLIENTINFO: //Get updated client info
				memcpy(&numClients,buff+5,sizeof(int)); //Get number of clients

				//Get clients
				for(int i = 0; i < numClients; i++)
				{
					memcpy(&clients[i],buff+9+i*sizeof(ClientInfo),sizeof(ClientInfo));
				}
				break;

			case ID:
				memcpy(&id,buff+5,sizeof(int)); //Get client's ID
				std::cout << "Your ID is " << id << "\n";
				break;

			case END:
				end = true;
				break;
			}
		}
	}
}

/**
 * Receives any TCP messages in the buffer
 */
void Client::getUDPMessages()
{
	char buff[1000];

	int err = 0;

	while(err != WSAEWOULDBLOCK)
	{
		//Find out the size of the message
		err = recv(sUDP, buff, 5, MSG_PEEK);
		if(err == -1)
		{
			err = WSAGetLastError();
		}

		if(err != WSAEWOULDBLOCK)
		{
			int size = *((int*)(buff+1)); //Get size

			//Get the message
			err = recv(sUDP, buff, size, 0);
			if(err == -1)
			{
				err = WSAGetLastError();
			}

			//Call the correct function depending on the message
			switch(buff[0])
			{
			case WORLDSTATE: //Get updated world state
				world = "";

				for(int i = 3; i < size; i++)
				{
					world.append(1, buff[i]);
				}
				break;
			}
		}
	}
}

int Client::sendTCPMessage(const char* message, int length)
{
	return send(sTCP,message,length, 0);
}

int Client::sendTCPMessage(std::string message)
{
	return send(sTCP,message.c_str(),message.length(), 0);
}

int Client::sendUDPMessage(const char* message, int length)
{
	return sendto(sUDP, message, length, 0, (SOCKADDR*) &target, sizeof(target));
}

/**
 * Lets the server know that the client is ready.
 */
int Client::ready()
{
	isReady = true;

	//Get size of message and put it in the buffer
	int size = 5;
	char* buffer = new char[size];
	buffer[0] = READY;
	memcpy(buffer+1,&size,sizeof(int));

	int err = sendTCPMessage(buffer,size);

	delete[] buffer;

	std::cout << "Readied up!\n";

	return err;
}

/**
 * Lets the server know that the client is not ready.
 */
int Client::unready()
{
	//Get size of message and put it in the buffer
	int size = 5;
	char* buffer = new char[size];
	buffer[0] = UNREADY;
	memcpy(buffer+1,&size,sizeof(int));

	int err = sendTCPMessage(buffer,size);

	delete[] buffer;

	return err;
}

/**
 * Sends the server the chosen colour
 */
int Client::setColor(int color)
{
	int size = 9;
	char* buffer = new char[size];
	buffer[0] = COLOR;
	memcpy(buffer+1,&size,sizeof(int));
	memcpy(buffer+5,&color,sizeof(int));

	int err = sendTCPMessage(buffer,size);

	delete[] buffer;

	return err;
}

/**
 * Sends the button state of the controller to the server.
 */
int Client::sendButtonState(Intention intention)
{
	std::string msg = intention.serialize(); //Get serialized button input

	//Put data in buffer
	//int size = msg.length() + 9;
	int size = sizeof(Intention) + 9;
	char* buffer = new char[size];
	buffer[0] = BUTTON;
	memcpy(buffer+1,&size,sizeof(int));
	memcpy(buffer+5,&id,sizeof(int));
	//memcpy(buffer+9,&msg,msg.length());
	memcpy(buffer+9,&intention,sizeof(Intention));

	int err = sendUDPMessage(buffer,size); //Send message

	delete[] buffer;

	return err;
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