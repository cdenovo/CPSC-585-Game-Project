#include "Waypoint.h"

Waypoint::Waypoint(IDirect3DDevice9* device, WPType _wpType)
{
	wpType = _wpType;
	switch(wpType)
	{
	case WAY_POINT:
		radius = 20;
		checkPointTime = 0;
		break;
	case CHECK_POINT:
		radius = 20;
		checkPointTime = 3;
	case LAP_POINT:
		radius = 20;
		checkPointTime = 3;
	case TURN_POINT:
		radius = 20;
		checkPointTime = 3;
	case SHARP_POINT:
		radius = 20;
		checkPointTime = 3;
	}

	drawable = new Drawable(WAYPOINT, "checker.dds", device);

	wpPosition = hkVector4(0,0,0);
}


Waypoint::~Waypoint(void)
{
	
}

void Waypoint::setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ)	// In Radians
{
	drawable->setPosAndRot(posX, posY, posZ,
		rotX, rotY, rotZ);

	wpPosition(0) = posX;
	wpPosition(1) = posY;
	wpPosition(2) = posZ;
}


void Waypoint::update()
{
	// Do nothing
}

float Waypoint::getRadius()
{
	return radius;
}

void Waypoint::setRadius(float newRadius)
{
	radius = newRadius;
}

int Waypoint::getCheckPointTime()
{
	return checkPointTime;
}

void Waypoint::setCheckPointTime(int seconds) 
{
	checkPointTime = seconds;
}

bool Waypoint::withinWaypoint(const hkVector4* position)
{
	hkSimdReal distance = wpPosition.distanceTo(*position);
	hkBool32 distanceLessThanRadius = distance.isLess(radius);

	if(distanceLessThanRadius){
		return true;
	}
	else{
		return false;
	}
}

WPType Waypoint::getWaypointType(){
	return wpType;
}
