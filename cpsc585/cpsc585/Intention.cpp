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
	leftStickX = 0;
	leftStickY = 0;
	rightTrig = 0;
	leftTrig = 0;

	acceleration = 0;
	cameraX = 0;
	cameraY = 0;
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
	rightStickX = 0;
	rightStickY = 0;
	leftStickX = 0;
	leftStickY = 0;
	rightTrig = 0;
	leftTrig = 0;

	acceleration = 0;
	cameraX = 0;
	cameraY = 0;
}


/**
* Converts intention into serialized data to be sent over a network
* Assumes bitstr is a size MESSAGELENGTH char array
*/
std::string Intention::serialize()
{
	std::string bitstr;

	//Initialize bitstring
	bitstr.append(MESSAGELENGTH, 0);

	bitstr[0] = 0x80 & (xPressed << 7); //Store xPressed
	bitstr[0] = bitstr[0] | (0x40 & (yPressed << 6)); //Store yPressed
	bitstr[0] = bitstr[0] | (0x20 & (aPressed << 5)); //Store aPressed
	bitstr[0] = bitstr[0] | (0x10 & (bPressed << 4)); //Store bPressed
	bitstr[0] = bitstr[0] | (0x08 & (selectPressed << 3)); //Store selectPressed
	bitstr[0] = bitstr[0] | (0x04 & (startPressed << 2)); //Store startPressed
	bitstr[0] = bitstr[0] | (0x02 & (lbumpPressed << 1)); //Store lbumpPressed
	bitstr[0] = bitstr[0] | (0x01 & rbumpPressed); //Store rbumpPressed

	//Store rightStickX
	bitstr[1] = (rightStickX & 0xFF00) >> 8;
	bitstr[2] = rightStickX & 0x00FF;

	//Store rightStickY
	bitstr[3] = (rightStickY & 0xFF00) >> 8;
	bitstr[4] = rightStickY & 0x00FF;

	//Store leftStick
	bitstr[5] = (leftStickX & 0xFF00) >> 8;
	bitstr[6] = leftStickX & 0x00FF;

	//Store rightTrig
	bitstr[7] = (rightTrig & 0xFF00) >> 8;
	bitstr[8] = rightTrig & 0x00FF;

	//Store leftTrig
	bitstr[9] = (leftTrig & 0xFF00) >> 8;
	bitstr[10] = leftTrig & 0x00FF;

	return bitstr;
}


/**
* Converts a serialized intention class back into the intention class
* Assumes that input bitstr is a char array of size MESSAGELENGTH
*/
void Intention::unserialize(char bitstr[])
{
	//Get boolean values (the ?true:false is to not get any compiler warnings about performance issues)
	xPressed =  (bitstr[0] & 0x80)?true:false;
	yPressed = (bitstr[0] & 0x40)?true:false;
	aPressed = (bitstr[0] & 0x20)?true:false;
	bPressed = (bitstr[0] & 0x10)?true:false;
	selectPressed = (bitstr[0] & 0x08)?true:false;
	startPressed = (bitstr[0] & 0x04)?true:false;
	lbumpPressed = (bitstr[0] & 0x02)?true:false;
	rbumpPressed = (bitstr[0] & 0x01)?true:false;

	rightStickX = ((int)bitstr[1] << 8) | bitstr[2]; //Get rightStickX
	rightStickY = ((int)bitstr[3] << 8) | bitstr[4]; //Get rightStickY
	leftStickX = ((int)bitstr[5] << 8) | bitstr[6]; //Get leftStickX
	rightTrig = ((int)bitstr[7] << 8) | bitstr[8]; //Get rightTrig
	leftTrig = ((int)bitstr[9] << 8) | bitstr[10]; //Get leftTrig
}


/**
* Converts the intention class into a string
*/
std::string Intention::toStr()
{

	std::stringstream ss;
	ss << "xPressed: " << xPressed << "\nyPressed: " << yPressed << std::endl;
	ss << "aPressed: " << aPressed << "\nbPressed: " << bPressed << std::endl;
	ss << "selectPressed: " << selectPressed << "\nstartPressed: " << startPressed << std::endl;
	ss << "lbumpPressed: " << lbumpPressed << "\nrbumpPressed: " << rbumpPressed << std::endl;

	ss << "rightStickX: " << rightStickX << "\nrightStickY: " << rightStickY << std::endl;
	ss << "leftStickX: " << leftStickX << std::endl;
	ss << "rightTrig: " << rightTrig << "\nleftTrig: " << leftTrig << std::endl;

	return ss.str();
}
