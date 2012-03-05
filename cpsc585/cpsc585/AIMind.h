#pragma once

#include "Racer.h"
#include "Waypoint.h"
#include "Ability.h"
#include "CheckpointTimer.h"
#include <time.h>

enum TypeOfRacer { PLAYER, COMPUTER };

class AIMind
{
public:
	AIMind(Racer* racer, TypeOfRacer _racerType);
	~AIMind(void);
	void update(HUD* hud, Intention intention, float seconds, Waypoint* waypoints[], Waypoint* checkpoints[]);
	void togglePlayerComputerAI();
	int getCheckpointTime();
	int getCurrentLap();
	int getCurrentWaypoint();
	int getSpeedCooldown();

private:
	void updateWaypointsAndLap(float seconds, Waypoint* waypoints[]);

	Racer* racer;

	Ability* speedBoost;

	CheckpointTimer* checkPointTimer;

	time_t oldTime;
	time_t newTime;

	hkVector4 lastPosition;

	TypeOfRacer racerType;

	int currentWaypoint;
	int checkPointTime;
	int currentLap;
};

