#include "DynamicObjManager.h"

DynamicObjManager* DynamicObjManager::manager = NULL;

DynamicObjManager::DynamicObjManager(void)
{
	objList = new std::vector<DynamicObj*>();
	objList->clear();
	objList->reserve(500);

	manager = this;
}


DynamicObjManager::~DynamicObjManager(void)
{
	manager = NULL;

	if (objList)
	{
		objList->clear();
		delete objList;
	}
}


void DynamicObjManager::update(float seconds)
{
	bool empty = true;

	if (!(objList->empty()))
	{
		for (std::vector<DynamicObj*>::iterator iter = objList->begin();
			iter < objList->end(); iter++)
		{
			if ((*iter) && (*iter)->destroyed)
			{
				(*iter)->~DynamicObj();
			}
			else if ((*iter) && !((*iter)->destroyed))
			{
				(*iter)->update(seconds);
				Renderer::renderer->addDynamicDrawable((*iter)->drawable);
				empty = false;
			}
		}
	}
	
	if (empty && !(objList->empty()))
	{
		objList->clear();
	}
}


void DynamicObjManager::addObject(DynamicObj* object)
{
	objList->push_back(object);
}
