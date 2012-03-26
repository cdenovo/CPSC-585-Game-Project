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
#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Convex/Cylinder/hkpCylinderShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppCompilerInput.h>
#include <Physics/Dynamics/hkpDynamics.h>
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/World/hkpWorldCinfo.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Dynamics/Entity/hkpRigidBodyCinfo.h>
#include <Physics/Dynamics/Constraint/ConstraintKit/hkpConstraintConstructionKit.h>
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>
#include <Physics/Collide/Filter/Group/hkpGroupFilter.h>
#include <Physics/Collide/Filter/Group/hkpGroupFilterSetup.h>
#include <Physics/Dynamics/Constraint/hkpConstraintInstance.h>
#include <Physics/Dynamics/Constraint/Util/hkpConstraintStabilizationUtil.h>
#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>
#include <Physics/Collide/Util/Welding/hkpMeshWeldingUtility.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Dynamics/Phantom/hkpAabbPhantom.h>


static void HK_CALL errorReport(const char* msg, void* userContext);

class Physics
{
public:
	Physics(void);
	~Physics(void);

	void initialize(int numObjects);
	void shutdown();
	void addRigidBody(hkpRigidBody *rb);
	void step(float seconds);

	void accelerate(float seconds, hkpRigidBody *racer, hkVector4 *accelerationVector);
	void rotate(float seconds, hkpRigidBody *racer, hkVector4 *rotVector);
	void steer(hkpRigidBody *racer, float steering);

	int getFilter();

	hkpWorld* world;

private:
	hkMemoryRouter* memoryRouter;
	int* filters;
	int numFilters;
	int currentFilter;
};
