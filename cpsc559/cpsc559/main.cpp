#include <iostream>
#include <string>
#include <WinSock.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <windows.h>

#include "Client.h"
#include "Server.h"

using namespace std;

void server()
{
	string command = "";
	Server s;

	while(command != "Q" && command != "q")
	{
		char ac[80];
		struct in_addr addr;
		if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
			cerr << "Error " << WSAGetLastError() <<
					" when getting local host name." << endl;
		}
		else
		{
			//cout << "Host name is " << ac << "." << endl;

			struct hostent *phe = gethostbyname(ac);
			if (phe == 0) {
				cerr << "Yow! Bad host lookup." << endl;
			}
			else
			{
				memcpy(&addr, phe->h_addr_list[0], sizeof(struct in_addr));
			}
		}
		cout << "Your IP address is " << inet_ntoa(addr) << ". (Press q to quit)" << endl;

		if(!s.listenOnPort(5869))
		{
			cout << "Error listening for connections.";
		}

		cout << "Connection recieved\n";
		while(true)
		{
			DWORD dwMillseconds=1000;
			Sleep(dwMillseconds);
			s.changeTrack("New Track");
		}

		//Get input
		cin.clear();
		cin >> command;
		if(command != "Q" && command != "q")
		{
			cout << "Command not recognized.\n";
		}			
	}
}

void client()
{
	Client c;
	string command = "";

	while(command != "Q" && command != "q")
	{
		cout << "Please enter the IP address of the server you wish to connect to or press q to quit.\n";

		//Get input
		cin >> command;
		if(command != "Q" && command != "q")
		{
			c.connectToServer(5869, command);

			while(true)
			{
				string track = c.track;
				c.getTCPMessages();

				if(track != c.track)
				{
					cout << "Track changed to " << c.track << endl;

					track = c.track;
					string temp;
					cin >> temp;
				}
			}
		}
	}
}

int main(int argc, const char* argv[])
{
	WSAData wsaData;
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        return 255;
    }

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
		client();
	}
	else //Run code for server
	{
		server();
	}

	WSACleanup();
	return 0;
}