// xrPhysics.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "xrPhysics.h"

#include "ode/memory.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

#ifdef DEBUG_MEMORY_MANAGER
static void* ode_alloc(size_t size)
{
    return Memory.mem_alloc(size, "ODE");
}
static void* ode_realloc(void* ptr, size_t oldsize, size_t newsize)
{
    return Memory.mem_realloc(ptr, newsize, "ODE");
}
static void ode_free(void* ptr, size_t size)
{
    return xr_free(ptr);
}
#else    // DEBUG_MEMORY_MANAGER
static void* ode_alloc(size_t size)
{
    return xr_malloc(size);
}
static void* ode_realloc(void* ptr, size_t oldsize, size_t newsize)
{
    return xr_realloc(ptr, newsize);
}
static void ode_free(void* ptr, size_t size)
{
    return xr_free(ptr);
}
#endif   // DEBUG_MEMORY_MANAGER

void XRPHYSICS_API PhysicsInitialize()
{
    dSetAllocHandler(ode_alloc);
    dSetReallocHandler(ode_realloc);
    dSetFreeHandler(ode_free);
}

#ifdef _MANAGED
#pragma managed(pop)
#endif
