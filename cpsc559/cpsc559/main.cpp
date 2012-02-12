#include <iostream>
#include <string>
#include <WinSock.h>
#include <windows.h>

#include "Client.h"
#include "Server.h"
#include "Input.h"

using namespace std;

LARGE_INTEGER frequency;        // ticks per second
LARGE_INTEGER t1, t2;           // ticks
double elapsedTime;

#define FPS 30

void server(Input *input)
{
	string command = "";
	Server s;

	s.setupWSA();
	s.setupTCPSocket(); //for game lobby

	while(command != "Q" && command != "q")
	{
		char ac[25]; //IPv6 will not have over 25 characters
		struct in_addr addr;

		//check the ip address to ensure it is legit
		if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
			cerr << "Error " << WSAGetLastError() <<
					" when getting local host name." << endl;
		}
		else
		{
			hostent *phe = gethostbyname(ac);
			if (phe == 0) {
				cerr << "Yow! Bad host lookup." << endl;
			}
			else
			{
				memcpy(&addr, phe->h_addr_list[0], sizeof(in_addr));
			}
		}
		cout << "Your IP address is " << inet_ntoa(addr) << "." << endl;

		while (!s.gameStarted){
			s.lobbyListen();	
		}

		s.setupUDPSocket();

		while (s.gameStarted){
			s.raceListen();

			Sleep(1000);
		}
	}
}

void client(Input *input)
{
	Client c;
	string command = "";
	bool success = false;

	while(command != "Q" && command != "q" && !success)
	{
		cout << "Please enter the IP address of the server you wish to connect to or press q to quit.\n";

		//Get input
		cin >> command;
		if(command != "Q" && command != "q")
		{
			success = c.connectToServer(5869, command);
		}
	}
	bool aPressed = false;
	bool aDown = false;
	bool bPressed = false;
	bool bDown = false;

	cout << "Press the A button when you are ready.\n";

	Intention intent;

	while(!intent.selectPressed)
	{
		QueryPerformanceCounter(&t1);
		if(c.start)
		{
			//c.getUDPMessages();
		}
		c.getTCPMessages();
				
		input->update();
		intent = input->getIntention();

		if(!c.start)
		{
			if(intent.aPressed)
			{
				if(!aPressed)
				{
					aDown = true;
				}
				else
				{
					aDown = false;
				}
				aPressed = true;
			}
			else
			{
				aDown = false;
				aPressed = false;
			}
			if(intent.bPressed)
			{
				if(!bPressed)
				{
					bDown = true;
				}
				else
				{
					bDown = false;
				}
				bPressed = true;
			}
			else
			{
				bDown = false;
				bPressed = false;
			}

			if(aDown && !c.isReady)
			{
				c.ready();
				cout << "Ready. Press B to cancel.\n";
			}
			else if(bDown && c.isReady)
			{
				c.unready();
				cout << "Not ready. Press A when you are ready.\n";
			}
		}
		else //The game has started
		{
			cout << "Enter button input (a/s/d/w): ";

			char inp;
			cin >> inp;

			switch (inp)
			{
			case 'a':
				intent.rightStickX = -25000;
				break;

			case 'd':
				intent.rightStickX = 25000;
				break;

			case 'w':
				intent.rightStickY = 25000;
				break;

			case 's':
				intent.rightStickY = -25000;
				break;
			}

			c.sendButtonState(intent);
		}

		QueryPerformanceCounter(&t2);
		elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
		double remaining = ((double)1)/FPS - elapsedTime;
		//if(remaining > 0)
		{
			//Sleep(remaining);
			Sleep(1000);
		}
	}
}

int main(int argc, const char* argv[])
{
	WSAData wsaData;
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        return 255;
    }

	QueryPerformanceFrequency(&frequency);

	Input *input = new Input();
	input->update();
	Intention intent = input->getIntention();

	cout << "Do you want to run this as a client or a server? (c/s)\n";

	string cs = "";

	while(cs != "c" && cs != "C" && cs != "s" && cs != "S")
	{
		cin >> cs;

		if(cs != "c" && cs != "C" && cs != "s" && cs != "S")
		{
			cout << "\nPlease enter c or s.\n";
		}
	}

	cout << endl;

	if(cs == "C" || cs == "c") //Run code for client
	{
		client(input);
	}
	else //Run code for server
	{
		server(input);
	}

	delete input;

	WSACleanup();
	return 0;
}