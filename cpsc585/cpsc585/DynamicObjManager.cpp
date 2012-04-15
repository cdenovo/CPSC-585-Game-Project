#include "DynamicObjManager.h"

DynamicObjManager* DynamicObjManager::manager = NULL;

DynamicObjManager::DynamicObjManager()
{
	objList = NULL;
	objList = new std::list<DynamicObj*>();
	objList->clear();
	manager = this;
}


DynamicObjManager::~DynamicObjManager()
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
	if (!(objList->empty()))
	{
		std::list<DynamicObj*>::iterator iter = objList->begin();
		
		DynamicObj* currentObj = NULL;

		while (iter != objList->end())
		{
			currentObj = (*iter);

			if (currentObj && currentObj->destroyed)
			{
				delete currentObj;
				iter = objList->erase(iter);
			}
			else if (currentObj && !(currentObj->destroyed))
			{
				Renderer::renderer->addDynamicDrawable(currentObj->drawable);
				currentObj->update(seconds);

				++iter;
			}
			else
			{
				++iter;
			}
		}
	}
}


void DynamicObjManager::addObject(DynamicObj* object)
{
	if (!object)
		return;

	objList->push_back(object);
}
