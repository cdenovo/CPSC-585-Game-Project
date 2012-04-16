#pragma once

#include <stdlib.h>
#include <math.h>

#include "Renderer.h"
#include "Input.h"
#include "Physics.h"
#include "Sound.h"
#include "Intention.h"
#include "Racer.h"
#include "World.h"
#include "Waypoint.h"
#include "AIMind.h"
#include "HUD.h"
#include "WaypointEditor.h"
#include "Ability.h"
#include "CheckpointTimer.h"
#include "DynamicObjManager.h"
#include "MenuHandler.h"

#define NUMRACERS 8
#define NUMWAYPOINTS 83
#define NUMCHECKPOINTS 4

class AI
{
public:
	AI();
	~AI();
	void shutdown();
	void initialize(Input* input);
	void simulate(float milliseconds);
	void displayDebugInfo(Intention intention, float milliseconds);
	void updateRacerPlacement(int left, int right);
	void initializeGame();

private:
	std::string getFPSString(float milliseconds);
	void initializeAIRacers();
	void initializeCheckpoints();
	void displayPostGameStatistics();
	std::string boolToString(bool boolean);
	std::string getSpaces(std::string input, int numSpaces);

	std::string postGameStatistics[9];
	bool generatePostGameStatistics;

	Input* input;
	Physics* physics;
	HUD* hud;
	MenuHandler* menuHandler;
	CheckpointTimer* checkPointTimer;
	WaypointEditor* wpEditor;

	DynamicObjManager* dynManager;

	int count;
	int fps;
	int currentWaypoint;

	// Race start/end stuff
	float raceStartTimer;
	bool raceStarted;
	bool raceEnded;
	bool playedOne, playedTwo, playedThree;

	bool paused;
	bool startReleased;

	int numberOfWaypoints;

	int racerIndex;

	// Abilities
	Ability* speedBoost;

	// Racers
	Racer* racers[NUMRACERS];
	Racer* player;
	Racer* ai1;
	Racer* ai2;
	Racer* ai3;
	Racer* ai4;
	Racer* ai5;
	Racer* ai6;
	Racer* ai7;

	// Racer Minds
	AIMind* racerMinds[NUMRACERS];
	AIMind* playerMind;
	AIMind* aiMind1;
	AIMind* aiMind2;
	AIMind* aiMind3;
	AIMind* aiMind4;
	AIMind* aiMind5;
	AIMind* aiMind6;
	AIMind* aiMind7;

	AIMind* racerPlacement[NUMRACERS];

	// World
	World* world;

	// Waypoints
	Waypoint* waypoints[NUMWAYPOINTS];

	Waypoint* buildingWaypoint;

	// Checkpoints
	Waypoint* checkpoints[NUMCHECKPOINTS];
	Waypoint* prevCheckpoints[NUMCHECKPOINTS];
};
