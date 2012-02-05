#include "AI.h"


AI::AI(void)
{
	renderer = NULL;
	input = NULL;
	physics = NULL;


	player = NULL;
	//aiVehicle1 = NULL;
}


AI::~AI(void)
{
}

void AI::shutdown()
{
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

	aiVehicle1 = new Racer(r->getDevice());
	r->addDrawable(aiVehicle1->drawable);
}

void AI::simulate(float milliseconds)
{
	_ASSERT(milliseconds > 0.0f);
	Intention intention = input->getIntention();

	// Debugging Information ---------------------------------------
	if(input->debugging()){
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
			std::string("LStick: ").append(buf5),};
	
		renderer->setText(stringArray, sizeof(stringArray) / sizeof(std::string));
	}
	else{
		std::string stringArray[] = {""};
		renderer->setText(stringArray, sizeof(stringArray) / sizeof(std::string));
	}

	// ---------------------------------------------------------------

	player->drawable->setPosAndRot(intention.leftStick / 1000.0f, 0, 0, 0, 0.0174f * (float) intention.rightTrig, 0);
	aiVehicle1->drawable->setPosAndRot(-5.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	
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