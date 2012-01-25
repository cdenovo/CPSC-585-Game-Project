#include "Intention.h"


Intention::Intention(void)
{
	xPressed = false;
	yPressed = false;
	aPressed = false;
	bPressed = false;
	selectPressed = false;
	startPressed = false;
	lbumpPressed = false;
	rbumpPressed = false;
	rightStickX = 0;
	rightStickY = 0;
	leftStick = 0;
	rightTrig = 0;
	leftTrig = 0;
}


Intention::~Intention(void)
{
}

void Intention::reset()
{
	xPressed = false;
	yPressed = false;
	aPressed = false;
	bPressed = false;
	selectPressed = false;
	startPressed = false;
	lbumpPressed = false;
	rbumpPressed = false;
}