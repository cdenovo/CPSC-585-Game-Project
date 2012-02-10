#pragma once

#include "Drawable.h"
#include "Physics.h"


class RearWheel
{
public:
	RearWheel(IDirect3DDevice9* device);
	~RearWheel(void);


private:


public:
	Drawable* drawable;
	hkpRigidBody* body;

private:


};

