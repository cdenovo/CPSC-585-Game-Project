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

#define NUMRACERS 8
#define NUMWAYPOINTS 83
#define NUMCHECKPOINTS 4

class AI
{
public:
	AI(void);
	~AI(void);
	void shutdown();
	void initialize(Renderer* renderer, Input* input, Sound* sound);
	void simulate(float milliseconds);
	void displayDebugInfo(Intention intention, float milliseconds);
	void updateRacerPlacement(int left, int right);

private:
	std::string getFPSString(float milliseconds);
	void initializeAIRacers();
	void initializeCheckpoints();
	void displayPostGameStatistics();
	std::string boolToString(bool boolean);

	Renderer* renderer;
	Input* input;
	Physics* physics;
	Sound* sound;
	HUD* hud;
	CheckpointTimer* checkPointTimer;
	WaypointEditor* wpEditor;

	DynamicObjManager* dynManager;

	int count;
	int fps;
	int currentWaypoint;

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

	// Checkpoints
	Waypoint* checkpoints[NUMCHECKPOINTS];
	Waypoint* prevCheckpoints[NUMCHECKPOINTS];
};
