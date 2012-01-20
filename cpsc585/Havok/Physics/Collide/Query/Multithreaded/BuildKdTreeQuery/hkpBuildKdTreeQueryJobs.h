/* 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's 
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok. 
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2011 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement. 
 */


#ifndef HK_BUILD_KD_TREE_JOBS_H
#define HK_BUILD_KD_TREE_JOBS_H

#include <Common/Base/hkBase.h>
#include <Common/Base/Thread/Semaphore/hkSemaphoreBusyWait.h>
#include <Common/Internal/KdTree/Build/hkKdTreeBuilder.h>
#include <Common/Internal/KdTree/Build/hkKdTreeBuildingUtils.h>

#include <Physics/Collide/Query/Multithreaded/CollisionQuery/hkpCollisionQueryJobs.h>
#include <Physics/Dynamics/World/hkpWorld.h>

typedef hkpCollisionQueryJobHeader hkpBuildKdTreeQueryJobHeader;

//
// The base class for all build kd-tree query jobs

struct hkpBuildKdTreeQueryJob : public hkJob
{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpBuildKdTreeQueryJob );

	// Kd-tree subtypes start at 128 to keep them separate from other path-finding jobs
	enum JobSubType
	{
		// Build jobs
		BUILD_KD_TREE_QUERY_FAST,
		BUILD_KD_TREE_QUERY_FAST_DISTRIBUTED_JOB,
		BUILD_KD_TREE_QUERY_FAST_DISTRIBUTED_SUBJOB,
		BUILD_KD_TREE_QUERY_SETUP_JOB,

		// Build coordination job, sets up other build jobs
		BUILD_KD_TREE_QUERY_COORDINATOR_JOB,

		BUILD_KD_TREE_QUERY_JOB_END
	};

	void atomicIncrementAndReleaseSemaphore() const;

protected:

	HK_FORCE_INLINE hkpBuildKdTreeQueryJob(JobSubType subType, hkUint16 size);

public:

	// This semaphore is released once the original job (and all its spawned children) has finished.
	hkSemaphoreBusyWait*			m_semaphore;

	// This header must be set for all jobs that potentially spawn additional jobs or that have been spawned by other jobs
	hkpBuildKdTreeQueryJobHeader*	m_sharedJobHeaderOnPpu;

	// The variable at this location will be incremented (atomically) when the job is done.
	hkUint32*						m_jobDoneFlag;

	// Needed by kd-tree building
	const hkpProcessCollisionInput*	m_collisionInput;
};

// ===============================================================================================================================================================================================
//   KD-TREE BUILDING
// ===============================================================================================================================================================================================

struct hkpKdTreeBuildJobFast : public hkpBuildKdTreeQueryJob
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpKdTreeBuildJobFast );

	HK_FORCE_INLINE hkpKdTreeBuildJobFast();
	HK_FORCE_INLINE hkpKdTreeBuildJobFast(hkKdTree* tree);

	int								m_numCollidables;

	class hkKdTreeNode*				m_treeOut;
	struct hkKdTreeProjectedEntry*	m_projEntOut;

	int*							m_treeDepthOut;

	int								m_nodeArraySize;
	int								m_numAvailableEmptyNodes;

	hkKdTreeProjectedEntry*			m_entriesIn;
	hkKdTreeProjectedEntry*			m_entriesOut;
};

struct hkpKdTreeBuildDistributedJob : public hkpBuildKdTreeQueryJob
{
public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpKdTreeBuildDistributedJob );

	
	/// PlayStation(R)3 note: the jobHeader and the m_colldiables and m_treeOut will be DMAd to SPU and therefore must not be allocated on PPU stack.
	HK_FORCE_INLINE hkpKdTreeBuildDistributedJob(hkpBuildKdTreeQueryJobHeader* jobHeader);
	HK_FORCE_INLINE hkpKdTreeBuildDistributedJob(hkpBuildKdTreeQueryJobHeader* jobHeader, hkKdTree* tree);

	hkKdTreeProjectedEntry*			m_primitivesIn;		// Should point to an aligned buffer in main memory of size numCollidables * sizeof(KdTreeBuilder::ProjectedEntry) 
	hkKdTreeProjectedEntry*			m_primitivesOut;	// Should point to an aligned buffer in main memory of size numCollidables * sizeof(KdTreeBuilder::ProjectedEntry) 

	class hkKdTreeNode*				m_treeOut;
	struct hkKdTreeProjectedEntry*	m_projEntOut;

	// depths of the 4 distributed branches
	int*							m_branchDepthsOut;
	int*							m_subJobSizes;

	int								m_numPrimitives;
	int								m_nodeArraySize;
	int								m_numAvailableEmptyNodes;
};

//
//	Creates 4 hkKdTreeBuildSetupJobs
//	The last hkpKdTreeBuildSetupJob to finish creates an hkpKdTreeBuildDistributedJob and adds it to the queue
//	The hkpKdTreeBuildDistributedJob creates 4 hkKdTreeBuildDistributedSubJobs

struct hkpKdTreeBuildCoordinatorJob: public hkpKdTreeBuildDistributedJob
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpKdTreeBuildCoordinatorJob );

	HK_FORCE_INLINE hkpKdTreeBuildCoordinatorJob(hkpBuildKdTreeQueryJobHeader* jobHeader, const hkpWorld* world);

	// Needed for hkpKdTreeBuildSetupJob
	// hkKdTreeBuildSetupJobs will use hkpKdTreeBuildCoordinatorJob::m_primitivesIn for their m_entriesOut
	const hkpCollidable* const*	m_collidables;
};

struct hkpKdTreeBuildSetupJob : public hkpKdTreeBuildDistributedJob
{
public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpKdTreeBuildSetupJob );

	HK_FORCE_INLINE hkpKdTreeBuildSetupJob();
	HK_FORCE_INLINE ~hkpKdTreeBuildSetupJob() {}

	const hkpCollidable* const*	m_collidables;
	hkKdTreeProjectedEntry* m_entriesOut;

	int						m_startIdx;
	int						m_endIdx;

	// Whether or not to spawn a new job when the last hkpKdTreeBuildSetupJob finishes
	int						m_spawnBuildDistributedJob;
};

//
//	This job is spawned internally by the hkpKdTreeBuildDistributedJob

struct hkpKdTreeBuildDistributedSubJob : public hkpBuildKdTreeQueryJob
{
public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpKdTreeBuildDistributedSubJob );

	HK_FORCE_INLINE hkpKdTreeBuildDistributedSubJob();

	class hkKdTreeNode*				m_treeOut;
	struct hkKdTreeProjectedEntry*	m_projEntOut;
	int								m_currentNodeIdx;
	hkKdTreeProjectedEntry*			m_primitivesIn;	
	hkKdTreeProjectedEntry*			m_primitivesOut;
	int								m_startPrimIdx;
	int								m_endPrimIdx;
	hkKdTreeProjectedEntry			m_bounds;
	int*							m_branchDepthOut;

	int								m_nodeArraySize;
	int								m_numAvailableEmptyNodes;
}; 

inline void hkSetupKdTreeBuildSubJob(const hkpKdTreeBuildDistributedJob& jobIn, const hkKdTreeBuildInput::BuildRecursiveInput& splitJob, hkpKdTreeBuildDistributedSubJob& subJobOut)
{
	subJobOut.m_sharedJobHeaderOnPpu	= jobIn.m_sharedJobHeaderOnPpu;
	subJobOut.m_primitivesIn			= splitJob.m_buffers->m_entriesIn;
	subJobOut.m_primitivesOut			= splitJob.m_buffers->m_entriesOut;
	subJobOut.m_currentNodeIdx			= splitJob.m_currentNodeIdx;
	subJobOut.m_nodeArraySize			= splitJob.m_maxNodeIdx;
	subJobOut.m_startPrimIdx			= splitJob.m_startPrimIdx;
	subJobOut.m_endPrimIdx				= splitJob.m_endPrimIdx;
	subJobOut.m_bounds					= splitJob.m_bounds;
	subJobOut.m_semaphore				= jobIn.m_semaphore;
	subJobOut.m_projEntOut				= jobIn.m_projEntOut;
}

//
//	Copy between hkpKdTreeBuildDistributedJob
//	Note that the size and subType aren't copied, so that we can go between subclasses

inline void hkInitKdTreeBuildJob(const hkpKdTreeBuildDistributedJob& jobIn, hkpKdTreeBuildDistributedJob& subJobOut)
{
	subJobOut.m_sharedJobHeaderOnPpu	= jobIn.m_sharedJobHeaderOnPpu;
	subJobOut.m_collisionInput			= jobIn.m_collisionInput;
	subJobOut.m_semaphore				= jobIn.m_semaphore;
	subJobOut.m_primitivesIn			= jobIn.m_primitivesIn;
	subJobOut.m_primitivesOut			= jobIn.m_primitivesOut;
	subJobOut.m_numPrimitives			= jobIn.m_numPrimitives;
	subJobOut.m_treeOut					= jobIn.m_treeOut;
	subJobOut.m_projEntOut				= jobIn.m_projEntOut;
	subJobOut.m_branchDepthsOut			= jobIn.m_branchDepthsOut;
	subJobOut.m_nodeArraySize			= jobIn.m_nodeArraySize;
	subJobOut.m_subJobSizes				= jobIn.m_subJobSizes;
	subJobOut.m_numAvailableEmptyNodes	= jobIn.m_numAvailableEmptyNodes;
}

#include <Physics/Collide/Query/Multithreaded/BuildKdTreeQuery/hkpBuildKdTreeQueryJobs.inl>

#endif // HK_COLLISION_JOBS_H

/* 
 * Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20110913) 
 *  
 * Confidential Information of Havok.  (C) Copyright 1999-2011 
 * Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok 
 * Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership 
 * rights, and intellectual property rights in the Havok software remain in 
 * Havok and/or its suppliers. 
 *  
 * Use of this software for evaluation purposes is subject to and indicates 
 * acceptance of the End User licence Agreement for this product. A copy of 
 * the license is included with this software and is also available at www.havok.com/tryhavok. 
 *  
 */
