/* 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's 
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok. 
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2011 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement. 
 */


HK_FORCE_INLINE hkResult hkcdClosestPointCapsuleCapsule(hkVector4Parameter capsA0, hkVector4Parameter capsA1, hkSimdRealParameter capsARadius, 
														hkVector4Parameter capsB0, hkVector4Parameter capsB1, hkSimdRealParameter capsBRadius, 
														hkSimdRealParameter collisionTolerance, hkVector4* HK_RESTRICT pointOut, hkVector4* HK_RESTRICT normalOut)
{
	hkVector4 dA; dA.setSub( capsA1, capsA0 );
	hkVector4 dB; dB.setSub( capsB1, capsB0 );

	hkVector4 closestPointA;
	hkVector4 closestAminusClosestB;
	hkSimdReal distanceSquared;
	hkVector4Comparison pointsNotUsed = hkcdClosestPointSegmentSegment( capsA0, dA, capsB0, dB, closestPointA, closestAminusClosestB, distanceSquared );

	const hkSimdReal radiusSum = capsARadius + capsBRadius;
	const hkSimdReal refDist = collisionTolerance + radiusSum;
	const hkSimdReal refDist2 = refDist * refDist; 
	if ( distanceSquared.isGreater( refDist2 ) )
	{
		return HK_FAILURE;
	}

	const hkSimdReal dist = distanceSquared.sqrt();
	const hkSimdReal epsilon = hkSimdReal::getConstant<HK_QUADREAL_EPS>();
	hkVector4 normal;
	
	while(1)
	{
		if ( pointsNotUsed.anyIsSet() ) // at least one endpoint involved
		{
			if ( distanceSquared.isGreaterZero() )
			{
				normal = closestAminusClosestB;
				break;
			}
		}
		else // interior of the segments
		{
			// try to get a better normal by using the cross product
			hkVector4 altNormal; altNormal.setCross( dA, dB );
			hkSimdReal altNormLen2 = altNormal.lengthSquared<3>();
			if ( altNormLen2.isGreater(epsilon) )
			{
				const hkSimdReal dot = altNormal.dot<3>( closestAminusClosestB );
				hkVector4 negNormal; negNormal.setNeg<4>( altNormal );
				normal.setSelect( dot.lessZero(), negNormal, altNormal );
				break;
			}
		}
		hkVector4Util::calculatePerpendicularVector( dA, normal );
		break;
	}

	normal.normalize<3>();
	normal.setComponent<3>( dist - radiusSum );
	hkVector4 point; point.setAddMul( closestPointA, normal, capsBRadius - dist );
	*normalOut = normal;
	*pointOut = point;
	
	return HK_SUCCESS;
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
