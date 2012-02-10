#include "AI.h"


AI::AI(void)
{
	renderer = NULL;
	input = NULL;
	physics = NULL;


	player = NULL;
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

	//Initialize physics
	physics = new Physics();
	physics->initialize();


	// You no longer have to manually add objects to the physics and renderer
	player = new Racer(r->getDevice(), renderer, physics);
	player->setPosAndRot(0.0f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	world = new World(r->getDevice(), renderer, physics);
	world->setPosAndRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);



	// This is how you set an object for the camera to focus on!
	renderer->setFocus(player->getIndex());
}

void AI::simulate(float seconds)
{
	_ASSERT(seconds > 0.0f);
	Intention intention = input->getIntention();

	// Debugging Information ---------------------------------------
	if(input->debugging())
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
			std::string("Steering: ").append(buf7)};
	
		renderer->setText(stringArray, sizeof(stringArray) / sizeof(std::string));
	}
	else{
		std::string stringArray[] = {""};
		renderer->setText(stringArray, sizeof(stringArray) / sizeof(std::string));
	}

	// ---------------------------------------------------------------
	
	player->steer(seconds, intention.steering);
	player->accelerate(seconds, intention.acceleration);

	


	/*hkVector4 zAcc = player->drawable->getZhkVector();
	zAcc.mul((float) intention.rightStickY);

	hkVector4 xAcc = player->drawable->getXhkVector();
	xAcc.mul((float) intention.rightStickX);

	hkVector4 yRot = player->drawable->getYhkVector();
	yRot.mul((float) intention.leftStick / 10.0f);
	
	physics->accelerate(seconds, player->body, &zAcc);
	physics->accelerate(seconds, player->body, &xAcc);
	physics->accelerate(seconds, player->body, &(hkVector4(0.0f, intention.rightTrig * 100.0f, 0.0f)));
	physics->rotate(seconds, player->body, &yRot);*/

	// To manipulate a Racer, you should use the methods Racer::accelerate(float) and Racer::steer(float)
	// Both inputs should be between -1.0 and 1.0. negative means backward or left, positive is forward or right.


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