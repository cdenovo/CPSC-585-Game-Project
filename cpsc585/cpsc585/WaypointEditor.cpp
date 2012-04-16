#include "WaypointEditor.h"


WaypointEditor::WaypointEditor(Renderer* _renderer)
{
	renderer = _renderer;
	waypointNumber = 0;
	prevX = 0;
	prevY = 0;
	prevZ = 0;
	waypoints = vector<Waypoint*>();
}


WaypointEditor::~WaypointEditor(void)
{
}



void WaypointEditor::writeToFile(vector<Waypoint*> _waypoints, int numberOfWaypoints, std::string fileName)
{
	file.open(fileName);
	if(file.is_open())
	{
		char numWaypoints[33];
		_itoa_s(numberOfWaypoints, numWaypoints, 10);
		file << numWaypoints << "\n";

		for(int i = 0; i < numberOfWaypoints; i++)
		{
			D3DXVECTOR3 wpPosition = _waypoints.at(i)->drawable->getPosition();
			char buf[33];
			_itoa_s((int)wpPosition.x, buf, 10);
			char buf2[33];
			_itoa_s((int)wpPosition.y, buf2, 10);
			char buf3[33];
			_itoa_s((int)wpPosition.z, buf3, 10);
			char buf4[33];
			_itoa_s(_waypoints.at(i)->getWaypointType(), buf4, 10);
			char buf5[33];
			_itoa_s((int)_waypoints.at(i)->getRadius(), buf5, 10);
			//      Xpos          Ypos           Zpos		    wpType		   wpRadius
			file << buf << '|' << buf2 << '|' << buf3 << '|' << buf4 << '|' << buf5 << "\n";
			
		}
	
		file.close();
	}
}

void WaypointEditor::loadWaypoints(Waypoint* _waypoints[], std::string fileName)
{
	string line;
	infile.open(fileName);
	//file.open(fileName);
	//ifstream infile (fileName);
	if(infile.is_open())
	{
		
		getline(infile, line); // Get number of waypoints
		//int numberOfWaypoints = atoi(line.c_str());
		
		int i = 0;
		while(infile.good()) // Make sure that the file it is reading from does not have an empty line at the end
		{
			getline(infile, line);
			
			int xPos = atoi((line.substr(0, line.find_first_of('|')).c_str())); line = line.substr(line.find_first_of('|')+1);
			int yPos = atoi((line.substr(0, line.find_first_of('|')).c_str())); line = line.substr(line.find_first_of('|')+1);
			int zPos = atoi((line.substr(0, line.find_first_of('|')).c_str())); line = line.substr(line.find_first_of('|')+1);
			int wpType = atoi((line.substr(0, line.find_first_of('|')).c_str())); line = line.substr(line.find_first_of('|')+1);
			int radius = atoi((line.substr(0, line.find_first_of('|')).c_str())); line = line.substr(line.find_first_of('|')+1);

			_waypoints[i] = new Waypoint(renderer->getDevice(), (WPType)wpType);
			_waypoints[i]->setPosAndRot((float)xPos, (float)yPos+15, (float)zPos, 0, 0, 0);
			_waypoints[i]->setRadius((float)radius);
			//renderer->addDrawable(_waypoints[i]->drawable);
			i++;
			
		}
		
		infile.close();

	}
	else{
		std::string stringArray[] = { "Failed To open file" };
		renderer->setText(stringArray, sizeof(stringArray) / sizeof(std::string));
	}
}