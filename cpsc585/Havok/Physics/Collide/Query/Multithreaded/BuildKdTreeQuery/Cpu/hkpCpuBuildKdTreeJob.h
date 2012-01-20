/* 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's 
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok. 
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2011 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement. 
 */


#include <Common/Base/hkBase.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>
#include <Common/Base/Spu/Util/hkSpuUtil.h>

#ifndef HK_CPU_BUILD_KD_TREE_JOB_H
#define HK_CPU_BUILD_KD_TREE_JOB_H

struct hkpProcessCollisionInput;
class hkJobQueue;

hkJobQueue::JobStatus HK_CALL hkpCpuKdTreeBuildCoordinatorJob(hkJobQueue& jobQueue, hkJobQueue::JobQueueEntry& nextJobOut);

hkJobQueue::JobStatus HK_CALL hkpCpuKdTreeBuildSetupJob(hkJobQueue& jobQueue, hkJobQueue::JobQueueEntry& nextJobOut);

hkJobQueue::JobStatus HK_CALL hkpCpuKdTreeBuildJobFast( hkJobQueue& jobQueue, hkJobQueue::JobQueueEntry& nextJobOut);

hkJobQueue::JobStatus HK_CALL hkpCpuKdTreeBuildFastDistributedJob(hkJobQueue& jobQueue, hkJobQueue::JobQueueEntry& nextJobOut);

hkJobQueue::JobStatus HK_CALL hkpCpuKdTreeBuildFastDistributedSubJob(hkJobQueue& jobQueue, hkJobQueue::JobQueueEntry& nextJobOut);

#endif	// HK_CPU_BUILD_KD_TREE_JOB_H

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
