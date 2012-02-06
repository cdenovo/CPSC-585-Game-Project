#pragma once

#include <iostream>
#include <sstream>

class Intention
{
public:
	Intention(void);
	~Intention(void);
	void reset();

	void serialize(char bitstr[]);
	void unserialize(char bitstr[]);

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

