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
	float frontExtents;
	float rearExtents;
	float frontDamping;
	float rearDamping;
	float chassisMass;
	float accelerationScale;
	float springForceCap;
	float grip;
	float topSpeed;
	std::string serverIP;
	bool inverse;

private:
	std::ifstream file;

	void parseLine(std::string line);
};

