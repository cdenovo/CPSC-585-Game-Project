#pragma once

#include <vector>
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
	std::vector<DynamicObj*>* objList;
};
