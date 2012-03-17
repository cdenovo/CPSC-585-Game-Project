#pragma once

#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <sstream>

#define MESSAGELENGTH 11

class Intention
{
public:
	Intention(void);
	~Intention(void);
	void reset();

	std::string serialize();
	void unserialize(char bitstr[]);

	std::string toStr();

	bool equals(Intention intent);

	// Buttons Pressed State
	bool xPressed;
	bool yPressed;
	bool aPressed;
	bool bPressed;
	bool selectPressed;
	bool startPressed;
	bool lbumpPressed;
	bool rbumpPressed;

	// Analog State
	int rightStickX;
	int rightStickY;
	int leftStick;
	int rightTrig;
	int leftTrig;


	float acceleration; // between -1.0 and 1.0
	float steering; // between -1.0 and 1.0
};
