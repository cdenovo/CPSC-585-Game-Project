#pragma once

#include "Drawable.h"
#include "Physics.h"

enum WPType { WAY_POINT, CHECK_POINT, LAP_POINT, TURN_POINT, SHARP_POINT };

class Waypoint
{
public:
	Waypoint(IDirect3DDevice9* device, WPType wpType);
	~Waypoint(void);
	void setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);	// In Radians
	void update();
	float getRadius();
	void setRadius(float newRadius);
	bool passedWaypoint(const hkVector4* nextPosition, const hkVector4* prevPosition, const hkVector4* bodyPosition);
	void setCheckPointTime(int seconds);
	int getCheckPointTime();
	WPType getWaypointType();

private:
	float radius;
	int checkPointTime;
	WPType wpType;

public:
	Drawable* drawable;
	hkVector4 wpPosition;
};
