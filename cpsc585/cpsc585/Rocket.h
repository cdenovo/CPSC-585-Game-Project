#pragma once

#include "Drawable.h"
#include "Physics.h"

class Rocket
{
public:
	Rocket();
	Rocket(IDirect3DDevice9* device);
	~Rocket(void);
	void update();
	void setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);	// In Radians
	void explode();

private:


public:
	Drawable* drawable;
	hkpRigidBody* body;
	bool destroyed;

private:


};
