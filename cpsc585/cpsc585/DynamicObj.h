#pragma once

#include "Drawable.h"
#include "Physics.h"

class DynamicObj
{
public:
	DynamicObj(void);
	virtual ~DynamicObj(void);
	virtual void update(float seconds) = 0;
	bool destroyed;

	Drawable* drawable;
};

