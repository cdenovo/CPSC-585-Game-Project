#include "Smoke.h"


Smoke::Smoke(IDirect3DDevice9* device)
{
	drawable = particle =  new Particle();

	radius = 1.0f;

	destroyed = false;

	lifetime = 2.8f;
}


Smoke::~Smoke(void)
{
	if (particle)
	{
		delete particle;
	}
}

void Smoke::initialize(IDirect3DDevice9* device, float rad)
{
	radius = rad;
	std::string frameList[] = { "textures/smoke1.dds" };

	particle->initialize(device, 1, frameList, rad, 2.8f, 5.0f, &hkVector4(0,0,0),
		&position, 2.8f);
	
}

void Smoke::setPos(hkVector4* pos)
{
	position.setXYZ(*pos);
}

void Smoke::update(float seconds)
{
	if (particle)
	{
		lifetime -= seconds;

		particle->update(seconds);
		
		if (lifetime <= 0.0f)
			destroyed = true;
	}
}
