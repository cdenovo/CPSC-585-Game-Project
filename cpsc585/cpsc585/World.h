#pragma once

#include "Drawable.h"
#include "Physics.h"
#include "Renderer.h"

class World
{
public:
	World(IDirect3DDevice9* device, Renderer* r, Physics* p);
	~World(void);
	void setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);	// In Radians

private:



public:
	Drawable* drawable;
	hkpRigidBody* body;
};

