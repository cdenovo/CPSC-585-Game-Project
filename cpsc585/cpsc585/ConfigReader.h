#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class ConfigReader
{
public:
	ConfigReader();
	~ConfigReader();

	float kFront;
	float kRear;
	float frontRestLength;
	float rearRestLength;
	float frontDamping;
	float rearDamping;
	float chassisMass;
	float frontWheelMass;
	float rearWheelMass;
	float accelerationScale;
	float steeringScale;
	float torqueScale;

private:
	std::ifstream file;

	void parseLine(std::string line);
};

