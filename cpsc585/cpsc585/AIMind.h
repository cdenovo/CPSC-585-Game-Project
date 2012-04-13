#pragma once

#include "Racer.h"
#include "Waypoint.h"
#include "Ability.h"
#include "CheckpointTimer.h"
#include "Physics.h"
#include "Renderer.h"
#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>

enum TypeOfRacer { PLAYER, COMPUTER };

class AIMind
{
public:
	AIMind(Racer* racer, TypeOfRacer _racerType, int NumberOfRacers, std::string _racerName);
	~AIMind(void);
	void update(HUD* hud, Intention intention, float seconds, Waypoint* waypoints[], Waypoint* checkpoints[], Waypoint* prevCheckpoints[], Racer* racers[], AIMind* racerPlacement[]);
	void togglePlayerComputerAI();
	void setPlacement(int place);
	int getPlacement();
	int getCheckpointTime();
	int getCurrentLap();
	int getCurrentWaypoint();
	int getSpeedCooldown();
	int getLaserLevel();
	int getSpeedLevel();
	int getOverallPosition();
	int getCurrentCheckpoint();
	int getRocketAmmo();
	int getSpeedAmmo();
	int getLandmineAmmo();

	int getKills();
	int getDeaths();
	int getSuicides();
	int getDamageDone();
	int getDamageTaken();

	float getRotationAngle();
	hkVector4 getRacerPosition();
	TypeOfRacer getTypeOfRacer();
	bool isfinishedRace();
	std::string getRacerName();

private:
	void updateWaypointsAndLap(float seconds, Waypoint* waypoints[]);
	void acquireAmmo();
	void upgrade();
	void downgrade();
	float calculateAngleToPosition(hkVector4* position);

	Racer* racer;

	Ability* speedBoost;
	Ability* laser;
	Ability* rocket;
	Ability* landmine;

	CheckpointTimer* checkPointTimer;

	time_t oldTime;
	time_t newTime;

	hkVector4 lastPosition;

	TypeOfRacer racerType;

	std::string racerName;

	int currentWaypoint;
	int checkPointTime;
	int overallPosition;
	int currentLap;
	int placement;
	int numberOfLapsToWin;
	int numberOfRacers;

	int knownNumberOfKills;
	int knownNumberOfDeaths;
	int knownNumberOfSuicides;
	int knownDamageDone;
	int knownDamageTaken;

	bool finishedRace;

	float rotationAngle;
};

