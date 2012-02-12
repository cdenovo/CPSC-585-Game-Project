#include "ConfigReader.h"

#define BUFFERSIZE 32

ConfigReader::ConfigReader()
{
	file.open("config.txt");

	if(file.is_open())
	{
		while(!file.eof())
		{
			char line[BUFFERSIZE];
			file.getline(line,BUFFERSIZE);

			parseLine(line);
		}

		file.close();
	}
}

ConfigReader::~ConfigReader()
{
}

/**
 * Parse the line of the config file
 */
void ConfigReader::parseLine(std::string line)
{
	int space = line.find(' ');
	if(space != std::string::npos) //If a space was found
	{
		std::string key = line.substr(0,space); //Get the key
		std::string value = line.substr(space+1,line.length()-key.length()-1); //Get the value

		std::istringstream ss(value);

		if(key == "KFRONT")
		{
			ss >> kFront; //Convert to float
		}
		else if(key == "KREAR")
		{
			ss >> kRear; //Convert to float
		}
		else if(key == "FRONTRESTLEN")
		{
			ss >> restLengthFront; //Convert to float
		}
		else if(key == "REARRESTLEN")
		{
			ss >> restLengthRear; //Convert to float
		}
		else if(key == "CHASSISMASS")
		{
			ss >> chassisMass; //Convert to float
		}
		else if(key == "FRONTWHEELMASS")
		{
			ss >> frontWheelMass; //Convert to float
		}
		else if(key == "REARWHEELMASS")
		{
			ss >> rearWheelMass; //Convert to float
		}
		else if(key == "ACCELERATIONSCALE")
		{
			ss >> accelerationScale; //Convert to float
		}
		else if(key == "STEERINGSCALE")
		{
			ss >> steeringScale; //Convert to float
		}
		else if(key == "TORQUESCALE")
		{
			ss >> torqueScale; //Convert to float
		}

	}
}
