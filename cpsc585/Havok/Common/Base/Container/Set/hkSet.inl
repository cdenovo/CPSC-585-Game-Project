/* 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's 
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok. 
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2011 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement. 
 */


template <typename T, typename Allocator>
inline hkBool32 hkSet<T, Allocator>::contains(T elem) const
{
	return m_storage.hasKey( elem );
}

template <typename T, typename Allocator>
inline hkBool32 hkSet<T, Allocator>::insert( T elem )
{
	return m_storage.insert(elem, true);
}

template <typename T, typename Allocator>
inline hkBool32 hkSet<T, Allocator>::tryInsert( T elem, hkResult& res )
{
	return m_storage.tryInsert( elem, true, res );
}

template <typename T, typename Allocator>
void hkSet<T, Allocator>::remove( Iterator it )
{
	m_storage.remove( it );
}

template <typename T, typename Allocator>
hkResult hkSet<T, Allocator>::remove( T elem )
{
	return m_storage.remove( elem );
}

template <typename T, typename Allocator>
int hkSet<T, Allocator>::getSize() const
{
	return m_storage.getSize();
}

template <typename T, typename Allocator>
typename hkSet<T, Allocator>::Iterator hkSet<T, Allocator>::getIterator() const
{
	return m_storage.getIterator();
}

template <typename T, typename Allocator>
T hkSet<T, Allocator>::getElement( Iterator i ) const
{
	return (T)m_storage.getKey(i);
}

template <typename T, typename Allocator>
typename hkSet<T, Allocator>::Iterator hkSet<T, Allocator>::getNext( Iterator i ) const
{
	return m_storage.getNext(i);
}

template <typename T, typename Allocator>
hkBool32 hkSet<T, Allocator>::isValid( Iterator i ) const
{
	return m_storage.isValid(i);
}

template <typename T, typename Allocator>
void hkSet<T, Allocator>::clear()
{
	m_storage.clear();
}

template <typename T, typename Allocator>
void hkSet<T, Allocator>::reserve( int numElements )
{
	m_storage.reserve(numElements);
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
