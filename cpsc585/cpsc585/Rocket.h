#pragma once

#include "Drawable.h"
#include "Physics.h"
#include "Sound.h"

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
	X3DAUDIO_EMITTER* emitter;

public:
	Drawable* drawable;
	hkpRigidBody* body;
	bool destroyed;

private:
	hkpContactListener* listener;
	IXAudio2SourceVoice* rocketVoice;

};

class RocketListener : public hkpContactListener
{
public:
	RocketListener(Rocket* r);
	void collisionAddedCallback(const hkpCollisionEvent& ev);

private:
	Rocket* rocket;
};
