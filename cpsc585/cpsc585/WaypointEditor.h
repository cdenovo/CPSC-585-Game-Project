#pragma once

#include "Intention.h"
#include "Racer.h"
#include <iostream>
#include <fstream>

using namespace std;

class WaypointEditor
{
public:
	WaypointEditor(Renderer* renderer);
	~WaypointEditor(void);
	void update(Racer* racer);
	void openFile();
	void closeFile();
private:
	int waypointNumber;
	ofstream file;
	Renderer* renderer;
	int prevX;
	int prevY;
	int prevZ;
};

