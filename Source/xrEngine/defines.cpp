#include "stdafx.h"

#ifdef DEBUG
ENGINE_API BOOL bDebug = FALSE;

#endif

// Video
//. u32			psCurrentMode		= 1024;
ENGINE_API u32     psCurrentVidMode[2] = {1024, 768};
ENGINE_API u32     psCurrentBPP        = 32;
// release version always has "mt_*" enabled
ENGINE_API Flags32 psDeviceFlags       = {
#ifndef DEBUG
    rsFullscreen |
#endif
    rsDetails | mtPhysics | mtSound | mtNetwork | rsDrawStatic | rsDrawDynamic | rsRefresh60hz};

// textures
ENGINE_API int psTextureLOD = 1;
