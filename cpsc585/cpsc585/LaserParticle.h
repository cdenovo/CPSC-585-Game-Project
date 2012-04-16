#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#include "SmokeParticle.h"

enum LaserType { LASER_BALL, LASER_FIRE };

class LaserParticle
{
public:
	LaserParticle();
	~LaserParticle();
	void initialize(IDirect3DDevice9* device, hkVector4* velocity, LaserType type, float time);
	void update(float seconds);
	void setPosition(hkVector4* pos);

	hkVector4 velocity;
	D3DXVECTOR3 position;


	float totalTime;
	bool destroyed;

	LaserType typeOfLaser;

private:
	float currentTime;
};
