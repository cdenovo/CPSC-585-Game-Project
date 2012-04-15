#pragma once

#include <list>
#include "DynamicObj.h"
#include "Renderer.h"

class DynamicObjManager
{
public:
	DynamicObjManager(void);
	~DynamicObjManager(void);
	void addObject(DynamicObj* object);
	void update(float seconds);

	static DynamicObjManager* manager;
private:
	std::list<DynamicObj*>* objList;
};
