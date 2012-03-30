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
	void processWindowsMsg(UINT umessage, WPARAM wparam, LPARAM lparam);
	bool update();
	Intention getIntention();
	bool debugging();
	bool placingWaypoint();
	void setPlaceWaypointFalse();

private:
	bool quit;
	bool debug;
	Intention intention;
	bool controllerAvailable;
	bool placeWaypoint;
};
