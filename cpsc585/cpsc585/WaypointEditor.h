#pragma once

#include "Intention.h"
#include "Racer.h"
#include "Waypoint.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class WaypointEditor
{
public:
	WaypointEditor(Renderer* renderer);
	~WaypointEditor(void);
	void writeToFile(vector<Waypoint*> _waypoints, int numberOfWaypoints, std::string fileName);
	void loadWaypoints(Waypoint* _waypoints[], std::string fileName);

private:
	int waypointNumber;
	ofstream file;
	ifstream infile;
	Renderer* renderer;
	int prevX;
	int prevY;
	int prevZ;

public:
	vector<Waypoint*> waypoints;
};

