#pragma once


// Put Havok includes here (Havok.h is included in the .cpp file to prevent redef's)
#include <stdio.h>
#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Base/Memory/System/hkMemorySystem.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>

#include <Physics/Collide/hkpCollide.h>
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppCompilerInput.h>
#include <Physics/Dynamics/hkpDynamics.h>
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/World/hkpWorldCinfo.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Dynamics/Entity/hkpRigidBodyCinfo.h>
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>



static void HK_CALL errorReport(const char* msg, void* userContext);

class Physics
{
public:
	Physics(void);
	~Physics(void);

	void initialize();
	void shutdown();
	void addRigidBody(hkpRigidBody *rb);

	void accelerate(hkpRigidBody *racer, float acceleration);
	void steer(hkpRigidBody *racer, float steering);

private:
	hkMemoryRouter* memoryRouter;
	hkpWorld* world;
};