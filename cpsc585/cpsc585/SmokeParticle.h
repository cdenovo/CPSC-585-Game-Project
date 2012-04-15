#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#include "Physics.h"

enum SmokeType { EXPLOSION_SMOKE, ROCKET_SMOKE };

struct ParticlePoint
{
	D3DXVECTOR3 pos;
	D3DCOLOR color;
};

class SmokeParticle
{
public:
	SmokeParticle();
	~SmokeParticle();
	void initialize(IDirect3DDevice9* device, hkVector4* velocity, float timeUntilFade, float fadingTimePeriod,
		int alpha, int red, int green, int blue, SmokeType type);
	void update(float seconds);
	void setPosition(hkVector4* pos);

	hkVector4 velocity;
	D3DXVECTOR3 position;
	D3DCOLOR color;
	float timeUntilFadeStart;
	float fadingTime;
	float fadingTimer;
	float totalTime;
	bool destroyed;

	SmokeType typeOfSmoke;

private:
	bool fading;
	float currentTime;
	int r, g, b;
	int alpha;
};
