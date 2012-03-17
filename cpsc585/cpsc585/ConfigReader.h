#pragma once

#define WIN32_LEAN_AND_MEAN
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
	float frontExtents;
	float rearExtents;
	float frontDamping;
	float rearDamping;
	float chassisMass;
	float accelerationScale;
	float springForceCap;
	std::string serverIP;
	float topSpeed;

private:
	std::ifstream file;

	void parseLine(std::string line);
};

