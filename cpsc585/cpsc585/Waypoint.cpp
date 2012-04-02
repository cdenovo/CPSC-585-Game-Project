#include "Waypoint.h"

Waypoint::Waypoint(IDirect3DDevice9* device, WPType _wpType)
{
	wpType = _wpType;
	switch(wpType)
	{
	case WAY_POINT:
		radius = 20;
		checkPointTime = 20;
		break;
	case CHECK_POINT:
		radius = 20;
		checkPointTime = 20;
	case LAP_POINT:
		radius = 20;
		checkPointTime = 20;
	case TURN_POINT:
		radius = 20;
		checkPointTime = 20;
	case SHARP_POINT:
		radius = 20;
		checkPointTime = 20;
	}

	drawable = new Drawable(WAYPOINT, "textures/checker.dds", device);

	wpPosition.set(0,0,0);
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

bool Waypoint::passedWaypoint(const hkVector4* nextPosition, const hkVector4* prevPosition, const hkVector4* bodyPosition)
{
	hkVector4 temp = *nextPosition;
	temp.sub(*prevPosition);
	hkVector4 A = temp;

	temp = *nextPosition;
	temp.sub(*bodyPosition);
	hkVector4 B = temp;

	const hkSimdReal result = A.dot3(B);

	
	if(result.isLess(0)){
		return true;
	}
	else{
		return false;
	}
}

WPType Waypoint::getWaypointType(){
	return wpType;
}
