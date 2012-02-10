#pragma once

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

	bool equals(const Intention &intent);

	std::string toStr();

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
};
