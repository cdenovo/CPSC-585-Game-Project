/* 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's 
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok. 
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2011 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement. 
 */


//
HK_FORCE_INLINE	hkBool32	hkcdRayDirectionAabbIntersect(const hkcdRay& ray, const hkAabb& aabb, hkSimdReal* fractionInOut)
{
	// Older version, fast.
	hkVector4 dNearTemp;	dNearTemp.setMul(aabb.m_min, ray.m_invDirection);
	hkVector4 dFarTemp;		dFarTemp.setMul(aabb.m_max, ray.m_invDirection);

	hkVector4 dNear;	dNear.setMin(dNearTemp, dFarTemp);
	hkVector4 dFar;		dFar.setMax(dNearTemp, dFarTemp);

	hkVector4 dNearY;	dNearY.setBroadcast<1>(dNear);
	hkVector4 dNearZ;	dNearZ.setBroadcast<2>(dNear);
	hkVector4 dFarY;	dFarY.setBroadcast<1>(dFar);
	hkVector4 dFarZ;	dFarZ.setBroadcast<2>(dFar);

	hkVector4 fraction;	fraction.setAll(*fractionInOut);
	hkVector4 nearYZ;	nearYZ.setMax(dNearY, dNearZ);
	hkVector4 farYZ;	farYZ.setMin(dFarY, dFarZ);
	hkVector4 nearX;	nearX.setMax(dNear, hkVector4::getZero());
	hkVector4 farX;		farX.setMin(dFar, fraction);

	nearX.setMax(nearX, nearYZ);
	farX.setMin(farX, farYZ);

	*fractionInOut = nearX.getComponent<0>();

	return nearX.lessEqual(farX).anyIsSet(hkVector4Comparison::MASK_X);
}

//
HK_FORCE_INLINE	hkBool32	hkcdIntersectRayAabb(const hkcdRay& ray, const hkAabb& aabb, hkSimdReal* fractionOut)
{
	hkAabb	relAabb;
	relAabb.m_min.setSub(aabb.m_min, ray.m_origin);
	relAabb.m_max.setSub(aabb.m_max, ray.m_origin);
	
	return hkcdRayDirectionAabbIntersect(ray, relAabb, fractionOut);
}

HK_FORCE_INLINE int hkcdIntersectRayBundleAabb(	const hkcdRay& ray, 
													const hkAabb& aabb0, const hkAabb& aabb1, const hkAabb& aabb2, const hkAabb& aabb3, 
													const hkSimdReal& earlyOutHitFraction)  
{
	hkVector4 dNearTemp0, dNearTemp1, dNearTemp2, dNearTemp3;
	hkVector4 dFarTemp0, dFarTemp1, dFarTemp2, dFarTemp3;

	dNearTemp0.setSub( aabb0.m_min, ray.m_origin );
	dFarTemp0.setSub( aabb0.m_max, ray.m_origin );
	dNearTemp1.setSub( aabb1.m_min, ray.m_origin );
	dFarTemp1.setSub( aabb1.m_max, ray.m_origin );
	dNearTemp2.setSub( aabb2.m_min, ray.m_origin );
	dFarTemp2.setSub( aabb2.m_max, ray.m_origin );
	dNearTemp3.setSub( aabb3.m_min, ray.m_origin );
	dFarTemp3.setSub( aabb3.m_max, ray.m_origin );

	dNearTemp0.mul( ray.m_invDirection );
	dFarTemp0.mul( ray.m_invDirection );
	dNearTemp1.mul( ray.m_invDirection );
	dFarTemp1.mul( ray.m_invDirection );
	dNearTemp2.mul( ray.m_invDirection );
	dFarTemp2.mul( ray.m_invDirection );
	dNearTemp3.mul( ray.m_invDirection );
	dFarTemp3.mul( ray.m_invDirection );

	hkVector4 dNear0; dNear0.setMin( dNearTemp0, dFarTemp0 );
	hkVector4 dFar0;  dFar0.setMax(  dNearTemp0, dFarTemp0 );
	hkVector4 dNear1; dNear1.setMin( dNearTemp1, dFarTemp1 );
	hkVector4 dFar1;  dFar1.setMax(  dNearTemp1, dFarTemp1 );
	hkVector4 dNear2; dNear2.setMin( dNearTemp2, dFarTemp2 );
	hkVector4 dFar2;  dFar2.setMax(  dNearTemp2, dFarTemp2 );
	hkVector4 dNear3; dNear3.setMin( dNearTemp3, dFarTemp3 );
	hkVector4 dFar3;  dFar3.setMax(  dNearTemp3, dFarTemp3 );

	//Transpose so we can do a combined min, max
	HK_TRANSPOSE4(dNear0, dNear1, dNear2, dNear3);
	HK_TRANSPOSE4(dFar0, dFar1, dFar2, dFar3);
	
	hkVector4 fraction; fraction.setAll(earlyOutHitFraction);

	dNear0.setMax(dNear0, dNear1);
	dNear2.setMax(dNear2, hkVector4::getZero());

	dFar0.setMin(dFar0, dFar1);
	dFar2.setMin(dFar2, fraction);

	dNear0.setMax(dNear0, dNear2);
	dFar0.setMin(dFar0, dFar2);
	
	hkVector4Comparison nearLtFar = dNear0.lessEqual(dFar0);
	return nearLtFar.getMask();
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
