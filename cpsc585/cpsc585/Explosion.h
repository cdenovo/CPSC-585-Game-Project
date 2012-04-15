#pragma once

#include "Racer.h"
#include "Physics.h"


#define BLAST_RADIUS 20.0
#define BLAST_DAMAGE 15.0

class Explosion
{
public:
	Explosion(hkTransform* trans, Racer* owns);
	~Explosion(void);
	void doDamage();

private:
	

public:
	Racer* owner;
	hkTransform* transform;

private:
	hkVector4 pos;
};
