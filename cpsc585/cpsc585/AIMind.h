#pragma once

#include "Racer.h"
#include "Waypoint.h"

class AIMind
{
public:
	AIMind(void);
	AIMind(Racer* racer);
	~AIMind(void);

	void update(float seconds, Waypoint* waypoints[]);

private:
	Racer* racer;

	int currentWaypoint;
};

