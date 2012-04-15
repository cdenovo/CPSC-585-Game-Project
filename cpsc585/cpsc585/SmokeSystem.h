#pragma once

#include <list>
#include "SmokeParticle.h"

#define MAX_ROCKET_SMOKE_PARTICLES 600
#define MAX_EXPLOSION_SMOKE_PARTICLES 200

class SmokeSystem
{
public:
	SmokeSystem();
	~SmokeSystem();
	void addSmoke(SmokeType type, SmokeParticle* particle);
	void update(float seconds);
	void render(SmokeType type);
	void cleanDestroyed();

	static SmokeSystem* system;


	inline DWORD FtoDw(float f)
	{
	    return *((DWORD*)&f);
	}


private:
	std::list<SmokeParticle*>* smokeList;

	IDirect3DTexture9* rocketSmokeTexture;
	IDirect3DTexture9* explosionSmokeTexture;

	int numRocketSmoke;
	int numExplosionSmoke;

	IDirect3DVertexBuffer9* rocketSmokeBuffer;
	IDirect3DVertexBuffer9* explosionSmokeBuffer;
};
