#pragma once

#include "Drawable.h"
#include "Physics.h"
#include "Sound.h"
#include "Explosion.h"
#include "DynamicObjManager.h"

class Racer;

class Landmine :
	public DynamicObj
{
public:
	Landmine(IDirect3DDevice9* device);
	~Landmine(void);
	void setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);	// In Radians
	void explode();
	void update(float seconds);
	void trigger();

private:
	X3DAUDIO_EMITTER* emitter;

public:
	Racer* owner;
	hkpRigidBody* body;
	bool activated;
	bool triggered;

private:
	hkpContactListener* listener;
	float activationTime;
	float triggeredTime;


};

class LandmineListener : public hkpContactListener
{
public:
	LandmineListener(Landmine* r);
	void collisionAddedCallback(const hkpCollisionEvent& ev);

private:
	Landmine* landmine;
};
