/* 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's 
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok. 
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2011 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement. 
 */


#ifndef HKP_BV_COMPRESSED_MESH_SHAPE_H
#define HKP_BV_COMPRESSED_MESH_SHAPE_H

//HK_HAVOK_ASSEMBLY_EXCLUDE_FILE

#include <Physics/Collide/Shape/Compound/Tree/hkpBvTreeShape.h>
#include <Physics/Collide/Util/Welding/hkpWeldingUtility.h>

struct hkGeometry;

extern const class hkClass hkpBvCompressedMeshShapeTreeClass;

	/// A shape representing a compressed triangulated geometry, stored in a bounding volume.
	/// It is not necessary to wrap this shape in any other bounding volume shapes.
	///
	/// This shape allows for per-triangle user data, collision filter information, and instrumented
	/// welding information. This per-triangle data must be provided during construction and cannot
	/// be changed later. Along with the source geometry, the ConstructionInfo structure has virtual
	/// functions which should be overridden if users wish to provide user data or filter info.
	/// Note that there is a limit of 256 distinct values for both user data and filter info.
	///
	/// Welding information will be computed during construction if ConstructionInfo::m_weldingType
	/// is specified. Note that this can take some time so should not be done at runtime.
	///
	/// NOTE: This feature is in BETA and may change in subsequent releases.
class hkpBvCompressedMeshShape : public hkpBvTreeShape, public hkpShapeContainer
{
	public:

			/// Structure used to create a hkpBvCompressedMeshShape
		struct ConstructionInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SHAPE, ConstructionInfo);

				/// Indicates how collision filter info or user data should be stored per triangle
			enum PerTriangleData
			{
				PER_TRIANGLE_DATA_NONE,
				PER_TRIANGLE_DATA_8_BIT,
				PER_TRIANGLE_DATA_PALETTE
			};

			ConstructionInfo( const hkGeometry& geometry, hkReal convexRadius = 0.0f );
			virtual ~ConstructionInfo() {}

				/// Callback to provide collision filter information for a given triangle,
				/// called when m_perTriangleCollisionFilterInfo != PER_TRIANGLE_DATA_NONE.
			virtual hkUint32 getTriangleCollisionFilterInfo(int triangleIndex) const { return 0; }

				/// Callback to provide user data for a given triangle,
				/// called when m_perTriangleUserData != PER_TRIANGLE_DATA_NONE.
			virtual hkUint32 getTriangleUserData(int triangleIndex) const { return 0; }

			const hkGeometry&					m_geometry;							///< Input geometry used to construct the compressed mesh
			hkReal								m_convexRadius;						///< Convex radius applied to each triangle
			hkReal								m_vertexWeldingTolerance;			///< Minimum distance between two vertices required to weld them together in the compression process. Defaults to zero.
			hkpWeldingUtility::WeldingType		m_weldingType;						///< Triangle welding type. Defaults to WELDING_TYPE_NONE.
			hkBool								m_weldOpenEdges;					///< If welding is used, snap contact normals in contacts with open edges to the triangle normal. Defaults to true.
			hkEnum<PerTriangleData, hkUint8>	m_perTriangleCollisionFilterInfo;	///< How per triangle collision filter will be stored. Defaults to PER_TRIANGLE_DATA_NONE.
			hkEnum<PerTriangleData, hkUint8>	m_perTriangleUserData;				///< How per triangle user data will be stored. Defaults to PER_TRIANGLE_DATA_NONE.
			hkArray<hkUint32>*					m_triangleToKeyMap;					///< If non NULL, on return it will contain a map between the input triangles and the child shape keys generated
		};

		enum
		{
				
			#if HK_POINTER_SIZE == 8
			NUM_BYTES_FOR_TREE = 160,
			#else
			NUM_BYTES_FOR_TREE = 144,
			#endif

				/// Maximum number of triangles in a mesh
			TRIANGLE_LIMIT = 1 << 23
		};

	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SHAPE );
		HK_DECLARE_REFLECTION();

			/// Constructor
		hkpBvCompressedMeshShape(const ConstructionInfo& cInfo);

			/// Finish constructor
		hkpBvCompressedMeshShape(hkFinishLoadedObjectFlag flag);

			/// Destructor
		virtual ~hkpBvCompressedMeshShape();

			/// Get the collision filter infos palette that is indexed with the triangle's 8-bit collision filter index
			/// to obtain the triangle collision filter info
		hkArray<hkUint32>& getCollisionFilterInfoPalette();

			/// Get the user data values palette that is indexed with the triangle's 8-bit user data index
			/// to obtain the triangle user data
		hkArray<hkUint32>& getUserDataPalette();

			/// Performs a linear cast and returns the shape keys of all intersected shapes
		int castAabb( const hkAabb& from, hkVector4Parameter to, hkpShapeKey* hits, int maxNumKeys ) const;

			/// Decompress the mesh into a geometry object (for visualization)
		void convertToGeometry( hkGeometry& geometryOut ) const;

			/// Returns the user data of a triangle
		hkUint32 getTriangleUserData(hkpShapeKey key) const;

		//
		// hkpShape interface
		//

		virtual const hkpShapeContainer* getContainer() const { return this; }

		virtual int calcSizeForSpu( const CalcSizeForSpuInput& input, int spuBufferSizeLeft ) const;

		HKP_SHAPE_VIRTUAL void getAabbImpl( HKP_SHAPE_VIRTUAL_THIS const hkTransform& localToWorld, hkReal tolerance, hkAabb& out ) HKP_SHAPE_VIRTUAL_CONST;

		HKP_SHAPE_VIRTUAL hkBool castRayImpl( HKP_SHAPE_VIRTUAL_THIS const hkpShapeRayCastInput& input, hkpShapeRayCastOutput& results ) HKP_SHAPE_VIRTUAL_CONST;

		HKP_SHAPE_VIRTUAL void castRayWithCollectorImpl( HKP_SHAPE_VIRTUAL_THIS const hkpShapeRayCastInput& input, const hkpCdBody& body, hkpRayHitCollector& collector ) HKP_SHAPE_VIRTUAL_CONST;

		//
		// hkpBvTreeShape interface
		//

		virtual void queryAabb( const hkAabb& aabb, hkArray<hkpShapeKey>& hits ) const;

		HKP_SHAPE_VIRTUAL hkUint32 queryAabbImpl( HKP_SHAPE_VIRTUAL_THIS const hkAabb& aabb, hkpShapeKey* hits, int maxNumKeys ) HKP_SHAPE_VIRTUAL_CONST;

		//
		// hkpShapeContainer interface
		//

		virtual int getNumChildShapes() const;

		virtual hkpShapeKey getFirstKey() const;		

		virtual hkpShapeKey getNextKey( hkpShapeKey oldKey ) const;		

		HKP_SHAPE_VIRTUAL const hkpShape* getChildShapeImpl( HKP_SHAPE_VIRTUAL_THIS hkpShapeKey key, hkpShapeBuffer& buffer ) HKP_SHAPE_VIRTUAL_CONST;

		HKP_SHAPE_VIRTUAL hkUint32 getCollisionFilterInfoImpl( HKP_SHAPE_VIRTUAL_THIS hkpShapeKey key ) HKP_SHAPE_VIRTUAL_CONST;

		//
		// SPU registration		
		//

		static void HK_CALL HK_INIT_FUNCTION( registerSimulationFunctions )( ShapeFuncs& sf );
		static void HK_CALL HK_INIT_FUNCTION( registerCollideQueryFunctions )( ShapeFuncs& sf );
		static void HK_CALL HK_INIT_FUNCTION( registerRayCastFunctions )( ShapeFuncs& sf );

	protected:

			/// Convex radius applied to each triangle
		hkReal m_convexRadius;

			/// Welding type applied to each triangle
		hkEnum<hkpWeldingUtility::WeldingType, hkUint8> m_weldingType; //+default(hkpWeldingUtility::WELDING_TYPE_NONE)

			/// Indicates if the shape has per triangle collision filter info. If this is false all triangles will have
			/// the default collision filter info value (0)
		hkBool m_hasPerTriangleCollisionFilterInfo;

			/// Indicates if the shape has per triangle user data. If this is false all triangles will have
			/// the default user data value (0)
		hkBool m_hasPerTriangleUserData;

			/// Collision filter info palette, indexed using the collision filter index stored with each triangle
		hkArray<hkUint32> m_collisionFilterInfoPalette;

			/// User data palette, indexed using the user data index stored with each triangle
		hkArray<hkUint32> m_userDataPalette;

			/// The static mesh tree
		HK_ALIGN16( hkUint8 m_tree[NUM_BYTES_FOR_TREE] ); //+overridetype(class hkpBvCompressedMeshShapeTree)

		friend struct hkpBvCompressedMeshShape_Internals;
};

#endif //HKP_BV_COMPRESSED_MESH_SHAPE_H

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
