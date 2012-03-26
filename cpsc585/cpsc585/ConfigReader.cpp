#include "ConfigReader.h"

#define BUFFERSIZE 32

ConfigReader::ConfigReader()
{
	//Initialize values
	kFront = 50000;
	kRear = 50000;
	frontExtents = 0.25;
	rearExtents = 0.25;
	frontDamping = 5000;
	rearDamping = 5000;
	chassisMass = 800;
	accelerationScale = 15;
	springForceCap = 80000;
	serverIP = "127.0.0.1";
	topSpeed = 100;
	grip = 2.0;
	inverse = false;


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
	else
	{
		std::ofstream outFile;
		outFile.open("config.txt");

		if(file.is_open())
		{
			outFile << "KFRONT " << kFront << "\n"
			<< "KREAR " << kRear << "\n"
			<< "FRONTEXTENTS " << frontExtents << "\n"
			<< "REAREXTENTS " << rearExtents << "\n"
			<< "FRONTDAMPING " << frontDamping << "\n"
			<< "REARDAMPING " << rearDamping << "\n"
			<< "CHASISMASS " << chassisMass << "\n"
			<< "ACCELERATIONSCALE " << accelerationScale << "\n"
			<< "SPRINGFORCECAP " << springForceCap << "\n"
			<< "SERVERIP " << serverIP << "\n"
			<< "TOPSPEED " << topSpeed << "\n"
			<< "GRIP " << grip
			<< "INVERSE " << (int) inverse;


			outFile.close();
		}
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
		else if(key == "FRONTDAMPING")
		{
			ss >> frontDamping; //Convert to float
		}
		else if(key == "REARDAMPING")
		{
			ss >> rearDamping; //Convert to float
		}
		else if(key == "FRONTEXTENTS")
		{
			ss >> frontExtents; //Convert to float
		}
		else if(key == "REAREXTENTS")
		{
			ss >> rearExtents; //Convert to float
		}
		else if(key == "CHASSISMASS")
		{
			ss >> chassisMass; //Convert to float
		}
		else if(key == "ACCELERATIONSCALE")
		{
			ss >> accelerationScale; //Convert to float
		}
		else if(key == "SPRINGFORCECAP")
		{
			ss >> springForceCap; //Convert to float
		}
		else if(key == "GRIP")
		{
			ss >> grip; //Convert to float
		}
		else if(key == "SERVERIP")
		{
			ss >> serverIP;
		}
		else if(key == "TOPSPEED")
		{
			ss >> topSpeed; //Convert to float
		}
		else if(key == "INVERSE")
		{
			ss >> inverse; //Convert to bool
		}
	}
}
