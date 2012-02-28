#pragma once
#pragma comment(lib, "XInput.lib")

#include <Windows.h>
#include <XInput.h>
#include "Intention.h"

#define THUMBSTICK_MAX 32767

class Input
{
public:
	Input(void);
	~Input(void);
	void processWindowsMsg(UINT umessage, WPARAM wparam);
	bool update();
	Intention getIntention();
	bool debugging();
	bool networking();
	bool isServer();
	bool isClient();

private:
	bool quit;
	bool network;
	bool client;
	bool server;
	bool debug;
	Intention intention;
	bool controllerAvailable;
};
