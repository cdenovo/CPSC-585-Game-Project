#include "LaserParticle.h"


LaserParticle::LaserParticle()
{
	velocity.set(0,0,0);
	position = D3DXVECTOR3(0,0,0);
	currentTime = 0.0f;
	totalTime = 0.0f;

	destroyed = false;
	typeOfLaser = LASER_BALL;
}


LaserParticle::~LaserParticle()
{

}

void LaserParticle::initialize(IDirect3DDevice9* device, hkVector4* vel, LaserType type, float time)
{
	typeOfLaser = type;
	velocity.setXYZ(*vel);

	totalTime = time;
	currentTime = 0.0f;
}


void LaserParticle::update(float seconds)
{
	currentTime += seconds;

	if (currentTime >= totalTime)
	{
		destroyed = true;
	}
	
	position.x += velocity(0) * seconds;
	position.y += velocity(1) * seconds;
	position.z += velocity(2) * seconds;
}


void LaserParticle::setPosition(hkVector4* pos)
{
	position.x = (*pos)(0);
	position.y = (*pos)(1);
	position.z = (*pos)(2);
}
