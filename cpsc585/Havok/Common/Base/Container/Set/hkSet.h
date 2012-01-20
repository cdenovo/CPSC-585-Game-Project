/* 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's 
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok. 
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2011 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement. 
 */


#ifndef HKBASE_SET_H
#define HKBASE_SET_H

#include <Common/Base/Container/PointerMap/hkPointerMap.h>


template <typename T, typename Allocator=hkContainerHeapAllocator>
class hkSet
{
public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_MAP, hkSet );

	typedef hkPointerMap<T, bool, Allocator> SetStorage;
	typedef typename SetStorage::Iterator Iterator;

	typedef hkSet<T,hkContainerTempAllocator> Temp;
	typedef hkSet<T,hkContainerDebugAllocator> Debug;
	typedef hkSet<T,hkContainerHeapAllocator> Heap;

		/// Check whether or not the set contains the element
	HK_FORCE_INLINE hkBool32 contains(T elem) const;

		/// Adds the element to the set. Returns true if the elem is new, or false if it was already in the set.
	HK_FORCE_INLINE hkBool32 insert( T elem );

		///	Try to insert element. Elements are unique and cannot be -1.
		/// Returns true if the elem is new, or false if it was already in the set.
		/// If insertion fails (due to being unable to expand the table), res is set to HK_FAILURE,
		/// otherwise it is set to HK_SUCCESS.
	HK_FORCE_INLINE hkBool32 tryInsert( T elem, hkResult& res );

		/// Remove element at it. The iterator is no longer valid after this call.
	void remove( Iterator it );

		/// If element present, remove it and return HK_SUCCESS. Otherwise return HK_FAILURE.
	hkResult remove( T elem );

	/// Return the number of elements in this map.
	int getSize() const;

		/// Get an iterator over the elements of this set.
	Iterator getIterator() const;

		/// Get the element at iterator i.
	T getElement( Iterator i ) const;

		/// Get the next iterator after i.
	Iterator getNext( Iterator i ) const;

		/// Return if the iterator has reached the end.
	hkBool32 isValid( Iterator i ) const;

		/// clear the set
	void clear();

		/// Reserve space for at least numElements;
	void reserve( int numElements );


private:
	SetStorage m_storage;
};

#include <Common/Base/Container/Set/hkSet.inl>

#endif // HKBASE_HKPOINTERMAP_H

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
