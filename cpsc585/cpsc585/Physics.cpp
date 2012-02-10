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

	// Initialize the physics world
	hkpWorldCinfo info;
	
	info.m_gravity.set(0.0f, -20.0f, 0.0f); // Set gravity (x,y,z)	[using 9.8 instead of ~20 looks bad]
	info.setBroadPhaseWorldSize(1000.0f); // Defaults to 1000.0f which is good in most cases
	info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_HARD);
	info.m_collisionTolerance = 0.01f;
	info.m_simulationType = simulationType;
	info.m_broadPhaseBorderBehaviour = info.BROADPHASE_BORDER_FIX_ENTITY;
	info.m_enableDeactivation = true;


	world = new hkpWorld(info);

	hkpAgentRegisterUtil::registerAllAgents( world->getCollisionDispatcher() );
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

void Physics::accelerate(float seconds, hkpRigidBody *racer, hkVector4 *accelVec)
{
	racer->applyForce(seconds, *accelVec); // Just for testing purposes
}

void Physics::rotate(float seconds, hkpRigidBody *racer, hkVector4 *rotVec)
{
	racer->applyTorque(seconds, *rotVec);
}

static void HK_CALL errorReport(const char* msg, void* userContext)
{
	printf("%s", msg);
}

void Physics::step(float seconds)
{
	world->stepDeltaTime(seconds);
}
