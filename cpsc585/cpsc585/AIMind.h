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

enum TypeOfRacer { PLAYER, COMPUTER, NETWORK, CLIENTPLAYER, CLIENTCOMP };

class AIMind
{
public:
	AIMind(Racer* racer, TypeOfRacer _racerType, int NumberOfRacers, std::string _racerName, std::string _colour);
	~AIMind(void);
	void update(HUD* hud, Intention intention, float seconds, Waypoint* waypoints[], Racer* racers[], AIMind* racerPlacement[], Waypoint* buildingWaypoint);
	void togglePlayerComputerAI(Waypoint* waypoints[]);
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
	void setTypeOfRacer(TypeOfRacer type);
	bool isfinishedRace();
	std::string getRacerName();
	std::string getRacerColour();
	void setRubberBanding(bool val);

	int numberOfLapsToWin;

private:
	void updateWaypointsAndLap(float seconds, Waypoint* waypoints[], Waypoint* buildingWaypoint);
	void acquireAmmo();
	void upgrade();
	void downgrade();
	void reset(Waypoint* waypoints[]);
	float calculateAngleToPosition(hkVector4* position);
	void runPlayerCode(HUD *hud, Intention intention, float seconds);
	void runAICode(float seconds, Waypoint* waypoints[], Racer* racers[], AIMind* racerPlacement[], Waypoint* buildingWaypoint);

	Racer* racer;

	Ability* speedBoost;
	Ability* laser;
	Ability* rocket;
	Ability* landmine;

	CheckpointTimer* checkPointTimer;

	time_t oldTime;
	time_t newTime;
	float spawnTime;
	float timeToNextTeleport;
	float timeSinceTeleport;

	hkVector4 lastPosition;

	TypeOfRacer racerType;

	std::string racerName;
	std::string colour;

	//int currentWaypoint;
	int checkPointTime;
	//int overallPosition;
	//int currentLap;
	//int placement;
	int numberOfRacers;

	int knownNumberOfKills;
	int knownNumberOfDeaths;
	int knownNumberOfSuicides;
	int knownDamageDone;
	int knownDamageTaken;

	bool finishedRace;
	bool teleportedRecently;

	bool playedNoAmmoSound;

	bool rubberBanding;

	float rotationAngle;

	float showAmmoTimer;

	HUD* racerHUD;
	HUD* playerHUD;
};

