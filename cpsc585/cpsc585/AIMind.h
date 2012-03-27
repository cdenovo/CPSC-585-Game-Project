#pragma once

#include "Racer.h"
#include "Waypoint.h"
#include "Ability.h"
#include "CheckpointTimer.h"
#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>

enum TypeOfRacer { PLAYER, COMPUTER };

class AIMind
{
public:
	AIMind(Racer* racer, TypeOfRacer _racerType);
	~AIMind(void);
	void update(HUD* hud, Intention intention, float seconds, Waypoint* waypoints[], Waypoint* checkpoints[], Waypoint* prevCheckpoints[], Racer* racers[]);
	void togglePlayerComputerAI();
	void setPlacement(int place);
	int getPlacement();
	int getCheckpointTime();
	int getCurrentLap();
	int getCurrentWaypoint();
	int getSpeedCooldown();
	int getLaserLevel();
	int getSpeedLevel();
	int getLaserDamage();
	int getOverallPosition();
	int getCurrentCheckpoint();
	float getRotationAngle();
	hkVector4 getRacerPosition();

private:
	void updateWaypointsAndLap(float seconds, Waypoint* waypoints[]);
	void upgrade();
	void downgrade();
	float calculateAngleToPosition(hkVector4* position);

	Racer* racer;

	Ability* speedBoost;
	Ability* laser;

	CheckpointTimer* checkPointTimer;

	time_t oldTime;
	time_t newTime;

	hkVector4 lastPosition;

	TypeOfRacer racerType;

	int currentWaypoint;
	int checkPointTime;
	int overallPosition;
	int currentLap;
	int placement;
	int knownNumberOfKills;
	int numberOfLapsToWin;

	float rotationAngle;
};

