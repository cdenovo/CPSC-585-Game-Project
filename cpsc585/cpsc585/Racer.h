#pragma once

#include "Drawable.h"
#include "Physics.h"


class Racer
{
public:
	Racer(IDirect3DDevice9* device);
	~Racer(void);
	void setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);	// In Radians
	void update();


private:



public:
	Drawable* drawable;
	hkpRigidBody* body;
};

