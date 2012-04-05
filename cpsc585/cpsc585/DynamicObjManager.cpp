#include "DynamicObjManager.h"

DynamicObjManager* DynamicObjManager::manager = NULL;

DynamicObjManager::DynamicObjManager(void)
{
	objList = new std::vector<DynamicObj*>();
	objList->clear();
	objList->reserve(100);

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
		std::vector<DynamicObj*>::size_type index = 0;
		while (index < objList->size())
		{
			if (((*objList)[index]) && (*objList)[index]->destroyed)
			{
				delete (*objList)[index];

				objList->erase(objList->begin() + index);
			}
			else if ((*objList)[index] && !((*objList)[index]->destroyed))
			{
				(*objList)[index]->update(seconds);
				Renderer::renderer->addDynamicDrawable((*objList)[index]->drawable);
				empty = false;

				index++;
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
