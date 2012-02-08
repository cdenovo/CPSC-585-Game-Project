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

	player = new Racer(r->getDevice());
	r->addDrawable(player->drawable);
	physics->addRigidBody(player->body);
	player->setPosAndRot(0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	world = new World(r->getDevice());
	r->addDrawable(world->drawable);
	physics->addRigidBody(world->body);
	world->setPosAndRot(0.0f, -10.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

void AI::simulate(float milliseconds)
{
	_ASSERT(milliseconds > 0.0f);
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
		
		std::string stringArray[] = { getFPSString(milliseconds), 
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
			std::string("LStick: ").append(buf5)};
	
		renderer->setText(stringArray, sizeof(stringArray) / sizeof(std::string));
	}
	else{
		std::string stringArray[] = {""};
		renderer->setText(stringArray, sizeof(stringArray) / sizeof(std::string));
	}

	// ---------------------------------------------------------------
	
	D3DXVECTOR3 zVec = player->drawable->getZVector();
	D3DXVECTOR3 xVec = player->drawable->getXVector();

	hkVector4 zAcc = hkVector4(zVec.x, zVec.y, zVec.z);
	zAcc.mul((float) intention.rightStickY);

	hkVector4 xAcc = hkVector4(xVec.x, xVec.y, xVec.z);
	xAcc.mul((float) intention.rightStickX);

	physics->accelerate(milliseconds, player->body, &zAcc);
	physics->accelerate(milliseconds, player->body, &xAcc);
	physics->accelerate(milliseconds, player->body, &(hkVector4(0.0f, intention.rightTrig * 100.0f, 0.0f)));
	physics->rotate(milliseconds, player->body, &(hkVector4(0.0f, intention.leftStick / 10.0f, 0.0f)));
	physics->step(milliseconds);
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