////////////////////////////////////////////////////////////////////////////
//	Module 		: xrGame.cpp
//	Created 	: 07.01.2001
//  Modified 	: 27.05.2004
//	Author		: Aleksandr Maksimchuk and Oles' Shyshkovtsov
//	Description : Defines the entry point for the DLL application.
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "object_factory.h"
#include "ui/xrUIXmlParser.h"
#include "xr_level_controller.h"
#include "profiler.h"

extern void CCC_RegisterCommands();
extern void setup_luabind_allocator();

extern void clean_game_globals();
extern void init_game_globals();
extern "C"
{
    DLL_API void __cdecl xrGameInitialize()
    {
        // PhysicsInitialize();
        CCC_RegisterCommands();
        // keyboard binding
        CCC_RegisterInput();

        setup_luabind_allocator();

#ifdef DEBUG
        g_profiler = xr_new<CProfiler>();
#endif
    }
    DLL_API DLL_Pure* __cdecl xrFactory_Create(CLASS_ID clsid)
    {
        DLL_Pure* object = object_factory().client_object(clsid);
#ifdef DEBUG
        if (!object)
            return (0);
#endif
        object->CLS_ID = clsid;
        return (object);
    }

    DLL_API void __cdecl xrFactory_Destroy(DLL_Pure* O)
    {
        xr_delete(O);
    }
};
