#pragma once

#include "Racer.h"

class AIMind
{
public:
	AIMind(void);
	AIMind(Racer* racer);
	~AIMind(void);

	Racer* racer;
};

