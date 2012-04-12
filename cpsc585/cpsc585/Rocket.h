#pragma once

#include "Drawable.h"
#include "Physics.h"
#include "Sound.h"
#include "Explosion.h"
#include "DynamicObjManager.h"

class Racer;

class Rocket :
	public DynamicObj
{
public:
	Rocket(IDirect3DDevice9* device, Racer* owner);
	~Rocket(void);
	void setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);	// In Radians
	void explode();
	void update(float seconds);

private:
	X3DAUDIO_EMITTER* emitter;

public:
	Racer* owner;
	hkpRigidBody* body;

private:
	hkpContactListener* listener;
	static IXAudio2SourceVoice* rocketVoice;
	float lifetime;


};

class RocketListener : public hkpContactListener
{
public:
	RocketListener(Rocket* r);
	void collisionAddedCallback(const hkpCollisionEvent& ev);

private:
	Rocket* rocket;
};
