#include "Client.h"

//new timeout feature (TAG1)
const int SERVER_TIMEOUT = 5;

Client::Client()
{
	start = false;
	end = false;
	isReady = false;
	id = -1;
	numClients = 0;
	newWorldInfo = false;
	udp_ready = false;
	wsa_ready = false;
	for(int i = 0; i < MAXCLIENTS; i++)
	{
		clients[i].millisecond_lag = 0;
	}
	seqNum = 0;
}

Client::~Client()
{
}

int Client::setupWSA()
{
	if (wsa_ready){
		std::cout << "Error: Attempting to setup Winsock but it is already setup!" << std::endl;
		return 0;
	}

	int error = WSAStartup(0x0202, &w); //Fill in WSA info

	if (error)
	{
		std::cout << "Error: error when calling WSAStartup()" << std::endl;
		wsa_ready = false;
		return error;
	}

	if(w.wVersion != 0x0202) //Wrong winsock version?
	{
		std::cout << "Error: Invalid version of Winsock, need 2.2." << std::endl;
		wsa_ready = false;
		WSACleanup();
		return 0;
	} 
	
	wsa_ready = true;
	std::cout << "WSA successfully setup." << std::endl;
	return 0;
}

int Client::setupUDPSocket()
{
	if (udp_ready)
	{
		std::cout << "Error: Attempting to setup a UDP socket when one is already setup!" << std::endl;
		return 0;
	}

	/*if (!wsa_ready){
		std::cout << "Error: Attempting to setup a UDP socket when WSA is not ready." << std::endl;
		return 0;
	}*/

	SOCKADDR_IN addr; //The address struct for TCP socket

	addr.sin_family = AF_INET; //Set address family
	addr.sin_port = htons(NETWORK_PORT); //Assign port to socket
	addr.sin_addr.s_addr = htonl(INADDR_ANY); //Accept connection from any IP
	sUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // Create socket

	if (sUDP == INVALID_SOCKET)
	{
		std::cout << "Error: Failed to create a valid UDP socket." << std::endl;
		//udp_ready = false;
		return 0;
	}

	if(bind(sUDP, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		std::cout << "Error: Failed to bind UDP socket." << std::endl;
		//udp_ready = false;
		return 0;
	}

	u_long iMode=1; //set FIONBIO arg to 1, which means making these sockets NON-BLOCKING
	if (ioctlsocket(sUDP,FIONBIO,&iMode) != 0){
		std::cout << "Error: Failed to make UDP socket non-blocking." << std::endl;
		//udp_ready = false;
		return 0;
	}

	//success
	//udp_ready = true;
	std::cout << "UDP socket successfully created." << std::endl;
	return 0; 
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

		sTCP = socket(AF_INET, SOCK_STREAM, 0); //Create TCP socket

		int nSocketBuffer = 131072; // 128k
		setsockopt(sTCP,SOL_SOCKET,SO_SNDBUF,(LPCSTR)&nSocketBuffer,sizeof(int));
		//setupUDPSocket();
		//sUDP = socket(AF_INET, SOCK_DGRAM, 0); // Create UDP socket

		if(sTCP != INVALID_SOCKET) //If we could create socket
		{
			//Try to connect
			/*std::stringstream ss;
			ss << port;

			struct addrinfo hint;
			struct addrinfo * result;
			memset( &hint, 0, sizeof(addrinfo) );
			hint.ai_family    = AF_UNSPEC;
			hint.ai_socktype  = SOCK_STREAM;
			hint.ai_flags     = AI_PASSIVE;*/

			//int s = getaddrinfo(ipAddress.c_str(),ss.str().c_str(),hint);

			if(connect(sTCP, (SOCKADDR*) &target, sizeof(target)) != SOCKET_ERROR)
			{
				std::cout << "Success!\n";
				success = true;
			}
			else
			{
				int error = WSAGetLastError();
				std::cout << "Fail!\n";
				//what should we do here?
			}

			//Set to non-blocking
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
void Client::getTCPMessages(float milliseconds)
{
	char buff[1000];

	int err = 0;
	bool hasError = false;

	//add milliseconds passed to lag timer for server (TAG1)
	clients[0].millisecond_lag += milliseconds;

	//check for timeout (TAG1)
	if (clients[0].millisecond_lag > SERVER_TIMEOUT)
	{
		clients[0].connected = false;
		std::cout << "Server timeout in getTCP" << std::endl;
	}

	while(!hasError)
	{
		//Find out the size of the message
		err = recv(sTCP, buff, 5, MSG_PEEK);



		if(err == -1)
		{
			err = WSAGetLastError();
			hasError = true;
			if(err != WSAEWOULDBLOCK)
			{
				clients[0].connected = false;
			}
		}

		if(!hasError)
		{
			//assumes there was no error (TAG1)
			clients[0].millisecond_lag = 0;

			int size = *((int*)(buff+1)); //Get size

			//Get the message
			err = recv(sTCP, buff, size, 0);
			if(err == -1)
			{
				err = WSAGetLastError();
				//error while reading tcp message (TAG1)
				std::cout << "Error receiving TCP from server." << std::endl;
				hasError = true;
				if(err != WSAEWOULDBLOCK)
				{
					clients[0].connected = false;
				}
			}
			else
			{
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
					{
					memcpy(reinterpret_cast<char *>(&numClients),buff+5,sizeof(int)); //Get number of clients

					//numClients = 2; //Trying to debug memory error, this shouldn't actually be here

					//Get clients
					for(int i = 0; i < numClients; i++)
					{
						memcpy(reinterpret_cast<char *>(&clients[i]),buff+9+i*sizeof(ClientInfo),sizeof(ClientInfo));
					}
					break;
					}

				case ID:
					{
					memcpy(reinterpret_cast<char *>(&id),buff+5,sizeof(int)); //Get client's ID
					std::cout << "Your ID is " << id << "\n";
					break;
					}

				case END:
					end = true;
					break;
				}
			}
		}
	}
}

/**
 * Receives any UDP messages in the buffer
 */
void Client::getUDPMessages(float milliseconds)
{
	char buff[1000];

	int err = 0;

	//add milliseconds passed to lag timer for server (TAG1)
	clients[0].millisecond_lag += milliseconds;

	//check for timeout (TAG1)
	if (clients[0].millisecond_lag > SERVER_TIMEOUT)
	{
		clients[0].connected = false;
		std::cout << "Server timeout in getUDP" << std::endl;
	}

	bool hasError = false;

	while(!hasError || err == 10040)
	{
		//Find out the size of the message
		err = recv(sUDP, buff, 5, MSG_PEEK);
		if(err == -1)
		{
			err = WSAGetLastError();
			hasError = true;
			if(err != WSAEWOULDBLOCK)
			{
				clients[0].connected = false;
			}
		}

		if(!hasError || err == 10040)
		{
			int size = *((int*)(buff+1)); //Get size

			//Get the message
			err = recv(sUDP, buff, size, 0);
			if(err == -1)
			{
				err = WSAGetLastError();
				hasError = true;
				//we should handle this properly (TAG1)
				if(err != WSAEWOULDBLOCK)
				{
					clients[0].connected = false;
				}
			}
			else
			{
				clients[0].millisecond_lag = 0;

				//Call the correct function depending on the message
				switch(buff[0])
				{
				case WORLDSTATE: //Get updated world state
					{
						int numRacers;
						int newSeqNum;
						memcpy(&newSeqNum,buff+5,sizeof(int)); //Get the new seqNum
						if(newSeqNum > seqNum)
						{
							seqNum = newSeqNum;
							memcpy(&numRacers,buff+9,sizeof(int)); //Get player's ID
							for(int i = 0; i < numRacers; i++)
							{
								memcpy(world[i],buff+9+i*RACERSIZE,RACERSIZE); //Get the button presses
							}

							newWorldInfo = true;
						}
						break;
					}
				}
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

//notifies server that this client is alive (TAG1)
int Client::sendAliveMessage()
{
	int size = 5;
	char* buffer = new char[size];
	buffer[0] = ALIVE;
	memcpy(buffer+1,&size,sizeof(int));
	int err = sendTCPMessage(buffer, size);
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
	if(sUDP)
	{
		closesocket(sUDP);
	}

	WSACleanup(); //Clean up Winsock
}