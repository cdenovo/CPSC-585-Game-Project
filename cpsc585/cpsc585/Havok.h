#pragma once
#pragma comment(lib, "hkBase.lib")
#pragma comment(lib, "hkSerialize.lib")
#pragma comment(lib, "hkSceneData.lib")
#pragma comment(lib, "hkInternal.lib")
#pragma comment(lib, "hkGeometryUtilities.lib")

#pragma comment(lib, "hkcdInternal.lib")

#pragma comment(lib, "hkpCollide.lib")
#pragma comment(lib, "hkpConstraintSolver.lib")
#pragma comment(lib, "hkpDynamics.lib")
#pragma comment(lib, "hkpInternal.lib")
#pragma comment(lib, "hkpUtilities.lib")

// Havok stuff
#include <Common/Base/hkBase.h>

#include <Common/Base/keycode.cxx>

#undef HK_FEATURE_PRODUCT_AI
#undef HK_FEATURE_PRODUCT_ANIMATION
#undef HK_FEATURE_PRODUCT_CLOTH
#undef HK_FEATURE_PRODUCT_DESTRUCTION
#undef HK_FEATURE_PRODUCT_BEHAVIOR

//#define HK_EXCLUDE_LIBRARY_hkcdInternal (apparently other parts of havok use this)
#define HK_EXCLUDE_LIBRARY_hkpVehicle
#define HK_EXCLUDE_LIBRARY_hkVisualize
#define HK_EXCLUDE_LIBRARY_hkCompat
#define HK_EXCLUDE_LIBRARY_hkgpConvexDecomposition

#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
#define HK_EXCLUDE_FEATURE_RegisterVersionPatches
#define HK_EXCLUDE_FEATURE_MemoryTracker

#define HK_EXCLUDE_FEATURE_hkpExtendedMeshShape
#define HK_EXCLUDE_FEATURE_hkpHeightField

#define HK_EXCLUDE_FEATURE_hkdCutoutFracture_runtimeFracture
#define HK_EXCLUDE_FEATURE_hkdDebrisFracture_runtimeFracture
#define HK_EXCLUDE_FEATURE_hkdDecomposeFracture_runtimeFracture
#define HK_EXCLUDE_FEATURE_hkdPieFracture_runtimeFracture
#define HK_EXCLUDE_FEATURE_hkdRandomSplitFracture_runtimeFracture
#define HK_EXCLUDE_FEATURE_hkdSliceFracture_runtimeFracture
#define HK_EXCLUDE_FEATURE_hkdSplitInHalfFracture_runtimeFracture
#define HK_EXCLUDE_FEATURE_hkdVoronoiFracture_runtimeFracture
#define HK_EXCLUDE_FEATURE_hkdWoodFracture_runtimeFracture
#define HK_EXCLUDE_FEATURE_hkdDecorateFractureFaceAction_filterPipeline
#define HK_EXCLUDE_FEATURE_hkdConvexDecompositionAction_filterPipeline
#define HK_EXCLUDE_FEATURE_hkdDecalMapAction_filterPipeline
#define HK_EXCLUDE_FEATURE_hkdGlueFixedPiecesAction_filterPipeline
#define HK_EXCLUDE_FEATURE_hkdFractureUtilRetriangulateRecursively

#include <Common/Base/Config/hkProductFeatures.cxx>


#include <Common/Base/KeyCode.h>
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkKeyCodeClasses.h>


#if defined(HK_PLATFORM_NGP)
unsigned int sceLibcHeapSize = 64*1024*1024;
#endif
