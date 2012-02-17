#pragma once

#include "Drawable.h"
#include "Physics.h"


class Waypoint
{
public:
	Waypoint(IDirect3DDevice9* device);
	~Waypoint(void);
	void setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);	// In Radians
	void update();
	float getRadius();
	void setRadius(float newRadius);
	bool withinWaypoint(const hkVector4* position);

private:
	float radius;


public:
	Drawable* drawable;
	hkpRigidBody* body;
};

