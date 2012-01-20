/* 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's 
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok. 
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2011 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement. 
 */


hkpBuildKdTreeQueryJob::hkpBuildKdTreeQueryJob( JobSubType subType,  hkUint16 size )
: hkJob( HK_JOB_TYPE_KD_TREE_BUILDING, subType, size, HK_JOB_SPU_TYPE_DISABLED ),
	m_semaphore(HK_NULL), m_jobDoneFlag(HK_NULL)
{
	m_jobSpuType = HK_JOB_SPU_TYPE_ENABLED; // all kd tree jobs run on the SPU
	m_sharedJobHeaderOnPpu = HK_NULL;
}

inline void hkpBuildKdTreeQueryJob::atomicIncrementAndReleaseSemaphore() const
{
	if (m_jobDoneFlag)
	{
		hkDmaManager::atomicExchangeAdd(m_jobDoneFlag, 1);
	}

	if (m_semaphore)
	{
		hkSemaphoreBusyWait::release(m_semaphore);
	}
}

hkpKdTreeBuildJobFast::hkpKdTreeBuildJobFast()
:	hkpBuildKdTreeQueryJob(BUILD_KD_TREE_QUERY_FAST, sizeof(hkpKdTreeBuildJobFast))
,	m_treeDepthOut(HK_NULL)
{}

hkpKdTreeBuildJobFast::hkpKdTreeBuildJobFast(hkKdTree *tree)
:	hkpBuildKdTreeQueryJob(BUILD_KD_TREE_QUERY_FAST, sizeof(hkpKdTreeBuildJobFast))
{
	m_numCollidables			= tree->getNumPrimitives();
	m_treeOut					= tree->getRoot();
	m_nodeArraySize				= tree->getTotalNumNodes();
	m_projEntOut				= tree->getProjectedEntries();
	m_numAvailableEmptyNodes	= tree->getNumReservedEmptyNodes();
	m_treeDepthOut				= &tree->m_maxDepth;
}

hkpKdTreeBuildDistributedJob::hkpKdTreeBuildDistributedJob(hkpBuildKdTreeQueryJobHeader* jobHeader)
:	hkpBuildKdTreeQueryJob(BUILD_KD_TREE_QUERY_FAST_DISTRIBUTED_JOB, sizeof(hkpKdTreeBuildDistributedJob))
,	m_branchDepthsOut(HK_NULL)
{
	m_sharedJobHeaderOnPpu	= jobHeader;
}

hkpKdTreeBuildDistributedJob::hkpKdTreeBuildDistributedJob(hkpBuildKdTreeQueryJobHeader* jobHeader, hkKdTree* tree)
:	hkpBuildKdTreeQueryJob(BUILD_KD_TREE_QUERY_FAST_DISTRIBUTED_JOB, sizeof(hkpKdTreeBuildDistributedJob))
,	m_branchDepthsOut(HK_NULL)
{
	m_sharedJobHeaderOnPpu		= jobHeader;
	m_numPrimitives				= tree->getNumPrimitives();
	m_treeOut					= tree->getRoot();
	m_nodeArraySize				= tree->getTotalNumNodes();
	m_projEntOut				= tree->getProjectedEntries();
	m_numAvailableEmptyNodes	= tree->getNumReservedEmptyNodes();
}

hkpKdTreeBuildDistributedSubJob::hkpKdTreeBuildDistributedSubJob()
:	hkpBuildKdTreeQueryJob(BUILD_KD_TREE_QUERY_FAST_DISTRIBUTED_SUBJOB, sizeof(hkpKdTreeBuildDistributedSubJob))
,	m_branchDepthOut(HK_NULL)
{}

hkpKdTreeBuildCoordinatorJob::hkpKdTreeBuildCoordinatorJob(hkpBuildKdTreeQueryJobHeader* jobHeader, const class hkpWorld* world)
:	hkpKdTreeBuildDistributedJob(jobHeader) 
{
	m_jobSubType = BUILD_KD_TREE_QUERY_COORDINATOR_JOB;
	m_size = sizeof(hkpKdTreeBuildCoordinatorJob);
	m_collisionInput = world->getCollisionInput();
}

hkpKdTreeBuildSetupJob::hkpKdTreeBuildSetupJob()
:	hkpKdTreeBuildDistributedJob(HK_NULL), 
	m_spawnBuildDistributedJob(false)
{
	m_jobSubType = BUILD_KD_TREE_QUERY_SETUP_JOB;
	m_size = sizeof(hkpKdTreeBuildSetupJob);
}

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
