#pragma once

#include "../xrCore/xrCore.h"
#include "../../xrAPI/xrAPI.h"

#ifndef MASTER_GOLD
#define INGAME_EDITOR
#endif

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

// you must define ENGINE_BUILD then building the engine itself
// and not define it if you are about to build DLL
#ifdef SHIPPING
#define ENGINE_API
#define DLL_API
#else
#ifndef NO_ENGINE_API
#ifdef ENGINE_BUILD
#define DLL_API    __declspec(dllimport)
#define ENGINE_API __declspec(dllexport)
#else
#undef DLL_API
#define DLL_API    __declspec(dllexport)
#define ENGINE_API __declspec(dllimport)
#endif
#else
#define ENGINE_API
#define DLL_API
#endif   // NO_ENGINE_API
#endif

#define ECORE_API

// Our headers
#include "engine.h"
#include "defines.h"
#ifndef NO_XRLOG
#include "../xrcore/log.h"
#endif
#include "xrDeviceInterface.h"
#include "../xrcore/fs.h"

#include "../xrcdb/xrXRC.h"

#include "../xrSound/sound.h"
#include "xrISEAbstract.h"
extern ENGINE_API CInifile* pGameIni;

#include "lua/library_linkage.h"
#include "luabind/library_linkage.h"

#ifndef DEBUG
#define LUABIND_NO_ERROR_CHECKING
#endif

#if !defined(DEBUG) || defined(FORCE_NO_EXCEPTIONS)
#define LUABIND_NO_EXCEPTIONS
#define BOOST_NO_EXCEPTIONS
#endif

#define LUABIND_DONT_COPY_STRINGS

#define READ_IF_EXISTS(ltx, method, section, name, default_value) (((ltx)->line_exist(section, name)) ? ((ltx)->method(section, name)) : (default_value))
