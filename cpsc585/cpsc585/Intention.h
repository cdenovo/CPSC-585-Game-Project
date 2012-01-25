#pragma once
class Intention
{
public:
	Intention(void);
	~Intention(void);
	void reset();

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

