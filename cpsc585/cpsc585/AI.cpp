#include "AI.h"


AI::AI(void)
{
	renderer = NULL;
	input = NULL;
	physics = NULL;


	player = NULL;
	ai1 = NULL;
	ai2 = NULL;
	ai3 = NULL;
	ai4 = NULL;
	world = NULL;
}


AI::~AI(void)
{
}

void AI::shutdown()
{
	if (player)
	{
		delete player;
		player = NULL;
	}
	
	if (ai1)
	{
		delete ai1;
		ai1 = NULL;
	}
	
	if(ai2)
	{
		delete ai2;
		ai2 = NULL;
	}

	if(ai3)
	{
		delete ai3;
		ai3 = NULL;
	}

	if(ai4)
	{
		delete ai4;
		ai4 = NULL;
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

	//Initialize player
	player = new Racer(r->getDevice(), renderer, physics, PLAYER);
	player->setPosAndRot(-40.0f, 5.0f, -40.0f, 0.0f, 0.0f, 0.0f);

	//Initialize world
	world = new World(r->getDevice(), renderer, physics);
	world->setPosAndRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	//Initialize AI-Racers
	initializeAIRacers();

	//Initialize Waypoints
	initializeWaypoints();

	// This is how you set an object for the camera to focus on!
	renderer->setFocus(player->getIndex());
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

	racerMinds[0] = aiMind1;
	racerMinds[1] = aiMind2;
	racerMinds[2] = aiMind3;
	racerMinds[3] = aiMind4;
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

void AI::simulate(float seconds)
{
	_ASSERT(seconds > 0.0f);
	Intention intention = input->getIntention();

	// Debugging Information ---------------------------------------
	if(input->debugging()){
		displayDebugInfo(intention, seconds);
	}
	else{
		std::string stringArray[] = {""};
		renderer->setText(stringArray, sizeof(stringArray) / sizeof(std::string));
	}
	// ---------------------------------------------------------------
	


	// To manipulate a Racer, you should use the methods Racer::accelerate(float) and Racer::steer(float)
	// Both inputs should be between -1.0 and 1.0. negative means backward or left, positive is forward or right.

	player->steer(seconds, intention.steering);
	player->accelerate(seconds, intention.acceleration);

	player->applySprings(seconds);
	player->update();

	for(int i = 0; i < 4; i++){
		racerMinds[i]->update(seconds, waypoints);
	}
	
	for(int i = 0; i < 4; i++){
		waypoints[i]->update();
	}

	updateHUD(intention);

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

	return;
}


void AI::updateHUD(Intention intention)
{
	// Code for updating the speedometer

	// Code for updating the Radial menu

	// Code for updating the checkpoint timer
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
