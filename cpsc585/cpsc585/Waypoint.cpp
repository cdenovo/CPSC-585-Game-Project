#include "Waypoint.h"



Waypoint::Waypoint(IDirect3DDevice9* device)
{
	radius = 20;

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
