#pragma once

#include "Drawable.h"
#include "Physics.h"


class FrontWheel
{
public:
	FrontWheel(IDirect3DDevice9* device, int filter);
	~FrontWheel(void);
	void update();

private:


public:
	Drawable* drawable;
	hkpRigidBody* body;

private:


};

