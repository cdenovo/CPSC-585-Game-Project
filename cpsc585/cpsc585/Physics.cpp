#include "Physics.h"
#include "Havok.h"	// Leave this define here, don't move it to Physics.h

Physics::Physics(void)
{
}


Physics::~Physics(void)
{
}

void Physics::initialize()
{
	memoryRouter = hkMemoryInitUtil::initDefault( hkMallocAllocator::m_defaultMallocAllocator, hkMemorySystem::FrameInfo( 500* 1024 ) );
	hkBaseSystem::init( memoryRouter, errorReport );
}

void Physics::shutdown()
{
	hkBaseSystem::quit();
	hkMemoryInitUtil::quit();
}