#pragma once

#include <stdlib.h>
#include <math.h>
#include <process.h>
#include <Windows.h>
#include <stdio.h>

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
#include "Server.h"
#include "Client.h"

#define NUMRACERS 5

class AI
{
public:
	AI(void);
	~AI(void);
	void shutdown();
	void initialize(Renderer* renderer, Input* input);
	void simulate(float milliseconds);
	void displayDebugInfo(Intention intention, float milliseconds);

private:
	std::string getFPSString(float milliseconds);
	void initializeWaypoints();
	void initializeAIRacers();
	std::string boolToString(bool boolean);

	//Networking functions
	static unsigned __stdcall staticGetIP(void * pThis);
	void getIP();
	static unsigned __stdcall staticConnectToServer(void *pThis);
	void connectToServer();
	void runNetworking();

	Renderer* renderer;
	Input* input;
	Physics* physics;
	HUD* hud;

	int count;
	int fps;
	int currentWaypoint;

	// Player
	Racer* player;

	// Racers
	Racer* racers[NUMRACERS];
	Racer* ai1;
	Racer* ai2;
	Racer* ai3;
	Racer* ai4;

	// Racer Minds
	AIMind* racerMinds[NUMRACERS];
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

	//Networking
	std::string hostName;
	HANDLE   hth1;
	HANDLE   hth2;
	Server server;
	Client client;
	ConfigReader config;
	bool clientConnected;
	bool serverStarted;
	bool tryToConnectToServer;
	bool readyPressed;
	Intention racerIntents[NUMRACERS];
	bool readyStatus[NUMRACERS];
	Intention prevIntent;
};
