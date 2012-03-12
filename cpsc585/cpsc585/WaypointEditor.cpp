#include "WaypointEditor.h"


WaypointEditor::WaypointEditor(Renderer* _renderer)
{
	renderer = _renderer;
	waypointNumber = 0;
	prevX = 0;
	prevY = 0;
	prevZ = 0;
}


WaypointEditor::~WaypointEditor(void)
{
}

void WaypointEditor::openFile()
{
	file.open("waypoints.txt");
}

void WaypointEditor::update(Racer* racer)
{
	if(file.is_open())
	{
		int x = (int)racer->body->getPosition().getComponent(0);
		int y = (int)racer->body->getPosition().getComponent(1);
		int z = (int)racer->body->getPosition().getComponent(2);

		if(prevX != x || prevY != y || prevZ != z)
		{
			waypointNumber++;
			char wpNum[33];
			_itoa_s(waypointNumber, wpNum, 10);
			char num1[33];
			_itoa_s(x, num1, 10);
			char num2[33];
			_itoa_s((y+5), num2, 10);
			char num3[33];
			_itoa_s(z, num3, 10);
			file << "wp" << wpNum << " = new Waypoint(renderer->getDevice(), CHECK_POINT);\n";
			file << "renderer->addDrawable(wp" << wpNum << "->drawable);\n";
			file << "wp" << wpNum << "->setPosAndRot(" << num1 << ".0f, " << num2 << ".0f, " << num3 << ".0f, 0.0f, 0.0f, 0.0f);\n";
			prevX = x; prevY = y; prevZ = z;
		}
	}
}

void WaypointEditor::closeFile()
{
	if(file.is_open())
	{
		for(int i = 1; i <= waypointNumber; i++)
		{
			char buf[33];
			_itoa_s(i-1, buf, 10);
			char buf2[33];
			_itoa_s(i, buf2, 10);
			file << "waypoints[" << buf << "] = wp" <<  buf2 << ";\n";
		}
	
		file.close();
	}
}
