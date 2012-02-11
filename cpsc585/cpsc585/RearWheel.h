#pragma once

#include "Drawable.h"
#include "Physics.h"


class RearWheel
{
public:
	RearWheel(IDirect3DDevice9* device, int filter);
	~RearWheel(void);
	void update();

private:


public:
	Drawable* drawable;
	hkpRigidBody* body;

private:


};

