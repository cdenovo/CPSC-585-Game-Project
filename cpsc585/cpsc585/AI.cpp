#include "AI.h"


AI::AI(void)
{
	renderer = NULL;
	input = NULL;
	physics = NULL;
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

	physics = new Physics();
	physics->initialize();
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

		string stringArray[] = { getFPSString(milliseconds), 
			"X: " + boolToString(intention.xPressed),
			"Y: " + boolToString(intention.yPressed),
			"A: " + boolToString(intention.aPressed),
			"B: " + boolToString(intention.bPressed),
			"Back: " + boolToString(intention.selectPressed),
			"Start: " + boolToString(intention.startPressed),
			string("Right Trigger: ").append(buf1),
			string("Left Trigger: ").append(buf2),
			string("RStick X: ").append(buf3),
			string("RStick Y: ").append(buf4),
			string("LStick: ").append(buf5),};
	
		renderer->setText(stringArray, sizeof(stringArray) / sizeof(string));
	}
	else{
		string stringArray[] = {""};
		renderer->setText(stringArray, sizeof(stringArray) / sizeof(string));
	}

	// ---------------------------------------------------------------

	
	return;
}


string AI::getFPSString(float milliseconds)
{
	count++;

	if (count > 20)
	{
		fps = (int) floor(1000.0f / milliseconds);
		count = 0;
	}
	
	char tmp[5];

	_itoa_s(fps, tmp, 5, 10);

	return string("FPS: ").append((const char*) tmp);
}

string AI::boolToString(bool boolean)
{
	if(boolean == true){
		return "True";
	}
	else {
		return "False";
	}
}