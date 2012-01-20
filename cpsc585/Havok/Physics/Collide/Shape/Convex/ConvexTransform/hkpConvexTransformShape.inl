/* 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's 
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok. 
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2011 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement. 
 */


inline const hkpConvexShape* hkpConvexTransformShape::getChildShape() const 
{
	return static_cast<const hkpConvexShape*>( m_childShape.getChild() ); 
}

inline void hkpConvexTransformShape::setTransform( const hkTransform& transform)
{
	m_transform.setFromTransformNoScale( transform );
}

inline void hkpConvexTransformShape::setTransform( const hkQsTransform& transform )
{
	HK_ON_DEBUG( hkSimdReal eps; eps.setFromFloat( HK_REAL_EPSILON ); );
	HK_ON_DEBUG( hkBool32 hasIdentityScale = transform.m_scale.allEqual<3>( hkVector4::getConstant<HK_QUADREAL_1>(), eps ) );
	HK_WARN_ON_DEBUG_IF( !hasIdentityScale && (getChildShape()->getType() != HK_SHAPE_CONVEX_VERTICES), 0x37b36bde, "Non-identity scale is not supported for child shapes other than hkpConvexVerticesShape" );
	m_transform = transform;
}

inline void hkpConvexTransformShape::getTransform( hkTransform* HK_RESTRICT transform ) const
{ 
	HK_ON_DEBUG( hkSimdReal eps; eps.setFromFloat( HK_REAL_EPSILON ); );
	HK_WARN_ON_DEBUG_IF( !m_transform.m_scale.allEqual<3>( hkVector4::getConstant<HK_QUADREAL_1>(), eps ), 0x37b36bdf, "Non-identity scale is being lost in hkQsTransform to hkTransform conversion" );
	m_transform.copyToTransformNoScale( *transform );
}

inline const hkQsTransform& hkpConvexTransformShape::getQsTransform() const 
{
	return m_transform; 
}

inline void hkpConvexTransformShape::initializeSpu( const hkpConvexShape* childShape, const hkQsTransform& transform, hkReal radius )
{
	
	m_type = HK_SHAPE_CONVEX_TRANSFORM;
	m_childShape = childShape; 
	m_radius = radius;
	m_transform = transform;
	m_childShapeSize = 0;
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
