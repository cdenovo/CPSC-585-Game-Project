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
}

void AI::simulate(float milliseconds)
{
	_ASSERT(milliseconds > 0.0f);

	string stringArray[] = { getFPSString(milliseconds), "Test test!",
		"another test!", "looooooooooooooooooooooooooooooong sentence!",
		"Eveeeeeeeeeeen looooooooooooooooooooooooooooooooooooonnnnnnnnnggggggggeeeeeeeeeeeeeeeeeeeeeeer" };
	
	renderer->setText(stringArray, sizeof(stringArray) / sizeof(string));




	
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