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


class AI
{
public:
	AI(void);
	~AI(void);
	void shutdown();
	void initialize(Renderer* renderer, Input* input);
	void initializeWaypoints();
	void initializeAIRacers();
	void simulate(float milliseconds);
	void displayDebugInfo(Intention intention, float milliseconds);
	void updateHUD(Intention intention);
	std::string boolToString(bool boolean);

private:
	std::string getFPSString(float milliseconds);

	Renderer* renderer;
	Input* input;
	Physics* physics;
	int count;
	int fps;
	int currentWaypoint;
	const hkVector4* blarg;

	// Player
	Racer* player;

	// Racers
	Racer* enemies[4];
	Racer* ai1;
	Racer* ai2;
	Racer* ai3;
	Racer* ai4;

	// Racer Minds
	AIMind* racerMinds[4];
	AIMind* aiMind1;
	AIMind* aiMind2;
	AIMind* aiMind3;
	AIMind* aiMind4;

	// World
	World* world;

	// Waypoints
	Waypoint* waypoints[4];
	Waypoint* wp1;
	Waypoint* wp2;
	Waypoint* wp3;
	Waypoint* wp4;
};
