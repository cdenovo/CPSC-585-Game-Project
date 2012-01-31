#pragma once

#include <iostream>
#include <Common/Base/hkBase.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
#include <Common/Base/Fwd/hkcstdio.h>

#if defined(HK_PLATFORM_NGP)
unsigned int sceLibcHeapSize = 64*1024*1024;
#endif

static void HK_CALL errorReport(const char* msg, void* userContext)
{
	using namespace std;
	printf("%s", msg);
}

class Physics
{
public:
	Physics(void);
	~Physics(void);
	void initialize();
	void shutdown();

private:
	hkMemoryRouter* memoryRouter;
	
};