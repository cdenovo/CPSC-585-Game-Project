#pragma once

#include "Drawable.h"
#include "Physics.h"

#define REARWHEELRAD 0.42f

class RearWheel
{
public:
	RearWheel(IDirect3DDevice9* device, int filter);
	~RearWheel(void);
	void update();
	void setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);	// In Radians

private:


public:
	Drawable* drawable;
	hkpRigidBody* body;
	bool touchingGround;

private:


};
