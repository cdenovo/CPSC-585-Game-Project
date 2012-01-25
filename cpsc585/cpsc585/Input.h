#pragma once
#pragma comment(lib, "XInput.lib")

#include <Windows.h>
#include <XInput.h>
#include "Intention.h"

class Input
{
public:
	Input(void);
	~Input(void);
	void processWindowsMsg(UINT umessage, WPARAM wparam);
	bool update();
	Intention getIntention();
	bool debugging();

private:
	bool quit;
	bool debug;
	Intention intention;
};
