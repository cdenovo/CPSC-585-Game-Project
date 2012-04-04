#pragma once
#include "dynamicobj.h"
#include "Particle.h"
class Smoke :
	public DynamicObj
{
public:
	Smoke(IDirect3DDevice9* device);
	~Smoke(void);
	void setPos(hkVector4* pos);
	void update(float seconds);
	void initialize(IDirect3DDevice9* device, float radius);

private:



public:
	float radius;

private:
	float lifetime;
	Particle* particle;
	hkVector4 position;

};

