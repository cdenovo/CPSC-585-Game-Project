#include "AI.h"
#include <iostream>

AI::AI(void)
{
	renderer = NULL;
	input = NULL;
	physics = NULL;
	hud = NULL;

	player = NULL;
	ai1 = NULL;
	ai2 = NULL;
	ai3 = NULL;
	ai4 = NULL;
	world = NULL;

	hostName = "";
	clientConnected = false;
	serverStarted = false;
	tryToConnectToServer = false;
	readyPressed = false;
}


AI::~AI(void)
{
}

void AI::shutdown()
{
	for(int i = 0; i < NUMRACERS; i++)
	{
		delete racers[i];
		racers[i] = NULL;

		delete racerMinds[i];
		racerMinds[i] = NULL;

		readyStatus[i] = false;
	}
	
	if (world)
	{
		delete world;
		world = NULL;
	}

	if (physics)
	{
		physics->shutdown();
		delete physics;
		physics = NULL;
	}

	if(hud)
	{
		//hud->shutdown();
		delete hud;
		hud = NULL;
	}

	//Clean up networking stuff
	//CloseHandle( hth1 ); //Close the handle to the thread that finds the IP address
	WSACleanup();
}

void AI::initialize(Renderer* r, Input* i)
{
	renderer = r;
	input = i;
	count = 25;
	fps = 0;

	// You no longer have to manually add objects to the physics and renderer

	//Initialize physics
	physics = new Physics();
	physics->initialize(5);
	
	hud = new HUD(r->getDevice(), RADIALMENU);
	renderer->addDrawable(hud->drawable);
	hud->setPosAndRot(-40.0f, 10.0f, -30.0f, 0.0f, 0.0f, 0.0f);
	

	//Initialize player
	player = new Racer(r->getDevice(), renderer, physics, PLAYER);
	player->setPosAndRot(-40.0f, 5.0f, -40.0f, 0.0f, 0.0f, 0.0f);
	racers[0] = player;
	racerMinds[0] = NULL;

	//Initialize world
	world = new World(r->getDevice(), renderer, physics);
	world->setPosAndRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	//Initialize AI-Racers
	initializeAIRacers();

	//Initialize Waypoints
	initializeWaypoints();

	// This is how you set an object for the camera to focus on!
	renderer->setFocus(player->getIndex());

	//Initialize winsock
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		//return 255;
	}
}

void AI::initializeAIRacers()
{
	ai1 = new Racer(renderer->getDevice(), renderer, physics, AI1);
	ai1->setPosAndRot(-40.0f, 5.0f, -30.0f, 0.0f, 0.0f, 0.0f);
	aiMind1 = new AIMind(ai1);
	
	ai2 = new Racer(renderer->getDevice(), renderer, physics, AI2);
	ai2->setPosAndRot(15.0f, 5.0f, 15.0f, 0.0f, 0.0f, 0.0f);
	aiMind2 = new AIMind(ai2);

	ai3 = new Racer(renderer->getDevice(), renderer, physics, AI3);
	ai3->setPosAndRot(-10.0f, 5.0f, 30.0f, 0.0f, 0.0f, 0.0f);
	aiMind3 = new AIMind(ai3);

	ai4 = new Racer(renderer->getDevice(), renderer, physics, AI4);
	ai4->setPosAndRot(10.0f, 5.0f, 30.0f, 0.0f, 0.0f, 0.0f);
	aiMind4 = new AIMind(ai4);

	racerMinds[1] = aiMind1;
	racerMinds[2] = aiMind2;
	racerMinds[3] = aiMind3;
	racerMinds[4] = aiMind4;

	racers[1] = ai1;
	racers[2] = ai2;
	racers[3] = ai3;
	racers[4] = ai4;
}

void AI::initializeWaypoints()
{
	wp1 = new Waypoint(renderer->getDevice());
	renderer->addDrawable(wp1->drawable);
	wp1->setPosAndRot(-40.0f, 5.0f, 40.0f, 0.0f, 0.0f, 0.0f);

	wp2 = new Waypoint(renderer->getDevice());
	renderer->addDrawable(wp2->drawable);
	wp2->setPosAndRot(40.0f, 5.0f, 40.0f, 0.0f, 0.0f, 0.0f);

	wp3 = new Waypoint(renderer->getDevice());
	renderer->addDrawable(wp3->drawable);
	wp3->setPosAndRot(40.0f, 5.0f, -40.0f, 0.0f, 0.0f, 0.0f);

	wp4 = new Waypoint(renderer->getDevice());
	renderer->addDrawable(wp4->drawable);
	wp4->setPosAndRot(-40.0f, 5.0f, -40.0f, 0.0f, 0.0f, 0.0f);

	waypoints[0] = wp1;
	waypoints[1] = wp2;
	waypoints[2] = wp3;
	waypoints[3] = wp4;
	
	currentWaypoint = 0;
}

unsigned __stdcall AI::staticGetIP(void * pThis)
{
    AI * pthX = (AI*)pThis;   // the tricky cast
    pthX->getIP();           // now call the true entry-point-function

    // A thread terminates automatically if it completes execution,
    // or it can terminate itself with a call to _endthread().

    return 1;          // the thread exit code
}

void AI::getIP()
{
    // This is the desired entry-point-function but to get
    // here we have to use a 2 step procedure involving
    // the ThreadStaticEntryPoint() function.
	//Get IP address
	server.setupWSA();
	server.setupTCPSocket(); //for game lobby
	server.setupUDPSocket();

	char ac[80];
	in_addr addr;
	std::stringstream ss;
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR)
	{
		ss << "Error " << WSAGetLastError() <<
				" when getting local host name.";
	}
	else
	{
		struct hostent *phe = gethostbyname(ac);
		if (phe == 0)
		{
			ss << "Bad host lookup.";
		}
		else
		{
			memcpy(&addr, phe->h_addr_list[0], sizeof(struct in_addr));
		}
	}
	if(ss.str().length() == 0)
	{
		ss << "Your IP address is " << inet_ntoa(addr) << ".";
	}
	hostName = ss.str();
}

unsigned __stdcall AI::staticConnectToServer(void * pThis)
{
    AI * pthX = (AI*)pThis;   // the tricky cast
    pthX->connectToServer();           // now call the true entry-point-function

    // A thread terminates automatically if it completes execution,
    // or it can terminate itself with a call to _endthread().

    return 1;          // the thread exit code
}

void AI::connectToServer()
{
    clientConnected = client.connectToServer(5869, config.serverIP);
	client.setupWSA();
	client.setupUDPSocket();
}

void AI::runNetworking()
{
	if(input->isServer())
	{
		if(!serverStarted)
		{
			unsigned  uiThread1ID;
			hth1 = (HANDLE)_beginthreadex( NULL,         // security
                                0,            // stack size
                                AI::staticGetIP,  // entry-point-function
                                this,           // arg list holding the "this" pointer
                                0,  //Thread starts right away
                                &uiThread1ID );

			hth1 = hth1;
			serverStarted = true;
		}
		else if(!server.gameStarted)
		{
			server.lobbyListen();
		}
		else
		{
			server.update(racers,NUMRACERS);
			server.raceListen();
		}
		std::stringstream ss;
		for(int i = 1; i < server.numClients; i++)
		{
			ss << "Player " << i << " has joined.\n";
			if(racerMinds[i] != NULL)
			{
				delete racerMinds[i];
				racerMinds[i] = NULL;
			}
		}
		std::string stringArray[] = {"You are a server.",hostName,ss.str()};
		renderer->setText(stringArray, sizeof(stringArray) / sizeof(std::string));
	}
	else if(input->isClient())
	{
		std::string msg1 = "";

		if(!clientConnected && !tryToConnectToServer)
		{
			std::stringstream ss;
			ss << "Connecting to server at IP address ";// << config.serverIP << "...";
			msg1 = ss.str();

			unsigned  uiThread1ID;
			tryToConnectToServer = true;
			hth2 = (HANDLE)_beginthreadex( NULL,         // security
                                0,            // stack size
                                AI::staticConnectToServer,  // entry-point-function
                                this,           // arg list holding the "this" pointer
                                0,  
                                &uiThread1ID );
		}
		else if(!clientConnected && tryToConnectToServer)
		{
			msg1 = "Could not connect to server.";
		}
		else if(clientConnected)
		{
			Intention intent = input->getIntention();
			if(!client.start)
			{
				//Check if the player already has their ID
				bool idFound = false;
				if(client.id >= 0)
				{
					idFound = true;
				}

				client.getTCPMessages();

				//If the player has been assigned an ID, set them to that car
				if(!idFound && client.id >= 0)
				{
					player = racers[client.id];
					renderer->setFocus(player->getIndex()); //Focus camera on player

					//Delete any minds for racers now over the network
					for(int i = 0; i < NUMRACERS; i++)
					{
						if(racerMinds[i] != NULL)
						{
							delete racerMinds[i];
							racerMinds[i] = NULL;
						}
					}
				}

				if(intent.rbumpPressed && !readyPressed)
				{
					client.ready();
					readyPressed = true;
				}
				else if(intent.lbumpPressed && readyPressed)
				{
					client.unready();
					readyPressed = false;
				}
				
				if(!client.isReady)
				{
					std::stringstream ss;
					ss << "Connected to server. Press the right bumper when you are ready.";
					msg1 = ss.str();
				}
				else
				{
					msg1 = "Ready. Waiting for game to start.";
				}

				std::stringstream ss;
				ss << "\n";
				//See if any other clients are ready
				int numClients = client.numClients;

				for(int i = 0; i < client.numClients; i++)
				{
					if(client.clients[i].ready)
					{
						ss << "Player " << client.clients[i].id << " is ready.\n";
					}
					else
					{
						ss << "Player " << client.clients[i].id << " is not ready.\n";
					}
				}
				msg1 = ss.str();
			}
			else
			{
				client.getUDPMessages();

				if(!intent.equals(prevIntent))
				{
					client.sendButtonState(intent);
				}
				msg1 = "Game started.";
			}
			prevIntent = intent;
		}

		std::string stringArray[] = {"You are a client.",msg1};
		renderer->setText(stringArray, sizeof(stringArray) / sizeof(std::string));
	}
	else
	{
		std::string stringArray[] = {"Press 'C' to become a client, or 'V' to become a server."};
		renderer->setText(stringArray, sizeof(stringArray) / sizeof(std::string));
	}
}

void AI::simulate(float seconds)
{
	_ASSERT(seconds > 0.0f);
	Intention intention = input->getIntention();

	// Debugging Information ---------------------------------------
	if(input->debugging()){
		displayDebugInfo(intention, seconds);
	}
	else if(input->networking())
	{
		runNetworking();
	}
	else
	{
		std::string stringArray[] = {""};
		renderer->setText(stringArray, sizeof(stringArray) / sizeof(std::string));
	}
	// ---------------------------------------------------------------
	
	
	// To manipulate a Racer, you should use the methods Racer::accelerate(float) and Racer::steer(float)
	// Both inputs should be between -1.0 and 1.0. negative means backward or left, positive is forward or right.

	for(int i = 0; i < NUMRACERS; i++)
	{
		if(racerMinds[i] != NULL)
		{
			racerMinds[i]->update(seconds, waypoints);
		}
		else if(player != racers[i] && server.gameStarted)
		{
			racers[i]->steer(seconds, server.intents[i].steering);
			racers[i]->accelerate(seconds, server.intents[i].acceleration);

			// Reset the player (in case you fall over)
			if (server.intents[i].yPressed)
			{
				D3DXVECTOR3 pos = racers[i]->drawable->getPosition();
				racers[i]->reset();
			}

			racers[i]->applyForces(seconds);
			racers[i]->update();
		}
		else if(client.newWorldInfo)
		{
			racers[i]->unserialize(client.world[i]);
			racers[i]->applyForces(seconds);
			racers[i]->update();
		}
	}
	client.newWorldInfo = false;

	player->steer(seconds, intention.steering);
	player->accelerate(seconds, intention.acceleration);

	player->applyForces(seconds);
	
	for(int i = 0; i < 4; i++){
		waypoints[i]->update();
	}
	// Update Heads Up Display
	hud->update(intention);

	// Switch focus (A for player, X for AI)
	if (intention.aPressed)
		renderer->setFocus(player->getIndex());
	else if (intention.xPressed)
		renderer->setFocus(ai1->getIndex());

	// Reset the player (in case you fall over)
	if (intention.yPressed)
	{
		D3DXVECTOR3 pos = player->drawable->getPosition();
		player->reset();
	}

	physics->step(seconds);

	player->update();

	return;
}


std::string AI::getFPSString(float milliseconds)
{
	count++;

	if (count > 20)
	{
		fps = (int) floor(1000.0f / milliseconds);
		count = 0;
	}
	
	char tmp[5];

	_itoa_s(fps, tmp, 5, 10);

	return std::string("FPS: ").append((const char*) tmp);
}

std::string AI::boolToString(bool boolean)
{
	if(boolean == true){
		return "True";
	}
	else {
		return "False";
	}
}

void AI::displayDebugInfo(Intention intention, float seconds)
{
		char buf1[33];
		_itoa_s(intention.rightTrig, buf1, 10);
		char buf2[33];
		_itoa_s(intention.leftTrig, buf2, 10);
		char buf3[33];
		_itoa_s(intention.rightStickX, buf3, 10);
		char buf4[33];
		_itoa_s(intention.rightStickY, buf4, 10);
		char buf5[33];
		_itoa_s(intention.leftStick, buf5, 10);
		char buf6[33];
		_itoa_s((int) (intention.acceleration * 100.0f), buf6, 10);
		char buf7[33];
		_itoa_s((int) (intention.steering * 100.0f), buf7, 10);

		char buf8[33];
		hkVector4 vel = player->body->getLinearVelocity();
		float velocity = vel.dot3(player->drawable->getZhkVector());
		_itoa_s((int) (velocity), buf8, 10);

		char buf9[33];
		_itoa_s((int) (Racer::accelerationScale), buf9, 10);
		
		std::string stringArray[] = { getFPSString(seconds * 1000.0f), 
			"X: " + boolToString(intention.xPressed),
			"Y: " + boolToString(intention.yPressed),
			"A: " + boolToString(intention.aPressed),
			"B: " + boolToString(intention.bPressed),
			"Back: " + boolToString(intention.selectPressed),
			"Start: " + boolToString(intention.startPressed),
			std::string("Right Trigger: ").append(buf1),
			std::string("Left Trigger: ").append(buf2),
			std::string("RStick X: ").append(buf3),
			std::string("RStick Y: ").append(buf4),
			std::string("LStick: ").append(buf5),
			std::string("Acceleration: ").append(buf6),
			std::string("Steering: ").append(buf7),
			std::string("Velocity: ").append(buf8),
			std::string("Accel. Scale: ").append(buf9)};
	
		renderer->setText(stringArray, sizeof(stringArray) / sizeof(std::string));
}
