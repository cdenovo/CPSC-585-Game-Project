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

	hkpWorldCinfo::SimulationType simulationType;
	simulationType = hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS;

	hkpWorldCinfo info;
	info.setBroadPhaseWorldSize( 5000.0f );
	info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_HARD);
	info.m_collisionTolerance = 0.01f;
	info.m_gravity = hkVector4( 0.0f,-9.8f,0.0f);
	info.m_simulationType = simulationType;
	info.m_broadPhaseBorderBehaviour = info.BROADPHASE_BORDER_FIX_ENTITY;
	info.m_enableDeactivation = true;

	world = new hkpWorld(info);
	world->lock();
}

void Physics::shutdown()
{
	hkBaseSystem::quit();
	hkMemoryInitUtil::quit();
}

void Physics::addRigidBody(hkpRigidBody *rb)
{
	world->addEntity( rb );
}

void Physics::accelerate(hkpRigidBody *racer, float acceleration)
{
	
}

static void HK_CALL errorReport(const char* msg, void* userContext)
{
	printf("%s", msg);
}
