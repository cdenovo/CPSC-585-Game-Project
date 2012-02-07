#pragma once

#include "Drawable.h"
#include "Physics.h"

class World
{
public:
	World(IDirect3DDevice9* device);
	~World(void);
	void setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);	// In Radians

private:



public:
	Drawable* drawable;
	hkpRigidBody* body;
};

