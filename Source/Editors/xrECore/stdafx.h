//----------------------------------------------------
// file: stdafx.h
//----------------------------------------------------
#pragma once
// DirectX headers
#include <d3d9.h>
#include "directx/d3dx9.h"
#include "../../xrRender/Private/xrD3dDefs.h"
#include "../xrCore/Utils.hpp"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
// #include <dsound.h>

#include "../xrEngine/stdafx.h"
#include "../xrEngine/xrDeviceInterface.h"
#include "../xrEProps/stdafx.h"
#include "../../xrCDB/xrCDB.h"
#include "../../xrSound/Sound.h"
#include "../../xrEngine/psystem.h"

#include "../../xrEngine/fmesh.h"
#include "../../xrEngine/_d3d_extensions.h"
#define smart_cast dynamic_cast

#ifndef O_SEQUENTIAL
#define O_SEQUENTIAL 0
#endif

#define R_R1    1
#define R_R2    2
#define RENDER  R_R1
#define REDITOR 1

#ifdef XRECORE_EXPORTS
#define ECORE_API __declspec(dllexport)
#else
#define ECORE_API __declspec(dllimport)
#endif

#define PropertyGP(a, b) __declspec(property(get = a, put = b))
#define THROW            FATAL("THROW");
#define THROW2(a)        R_ASSERT(a);
#define clMsg            Msg

class PropValue;
class PropItem;
DEFINE_VECTOR(PropItem*, PropItemVec, PropItemIt);

class ListItem;
DEFINE_VECTOR(ListItem*, ListItemsVec, ListItemsIt);

// some user components

DEFINE_VECTOR(xr_string, AStringVec, AStringIt);
DEFINE_VECTOR(xr_string*, LPAStringVec, LPAStringIt);

#include "../Public/xrEProps.h"
#include "../../xrCore/Log.h"
#include "editor/ELog.h"
#include "../../xrEngine/defines.h"

#include "../../xrphysics/xrphysics.h"
#include "../../xrRender/Private/FVF.h"

struct str_pred
{
    IC bool operator()(LPCSTR x, LPCSTR y) const
    {
        return strcmp(x, y) < 0;
    }
};
struct astr_pred
{
    IC bool operator()(const xr_string& x, const xr_string& y) const
    {
        return x < y;
    }
};

#include "editor/device.h"
#include "../../xrEngine/properties.h"
#include "editor/render.h"
DEFINE_VECTOR(FVF::L, FLvertexVec, FLvertexIt);
DEFINE_VECTOR(FVF::TL, FTLvertexVec, FTLvertexIt);
DEFINE_VECTOR(FVF::LIT, FLITvertexVec, FLITvertexIt);
DEFINE_VECTOR(shared_str, RStrVec, RStrVecIt);

#include "Editor/EditorPreferences.h"

#ifdef _LEVEL_EDITOR
#include "net_utils.h"
#endif

#define INI_NAME(buf)                                                                             \
    {                                                                                             \
        FS.update_path(buf, "$local_root$", EFS.ChangeFileExt(UI->EditorName(), ".ini").c_str()); \
    }
// #define INI_NAME(buf) 		{buf =
// buf+xr_string(Core.WorkingPath)+xr_string("\\")+EFS.ChangeFileExt(UI->EditorName(),".ini");}
#define DEFINE_INI(storage)         \
    {                               \
        string_path buf;            \
        INI_NAME(buf);              \
        storage->IniFileName = buf; \
    }
#define NONE_CAPTION        "<none>"
#define MULTIPLESEL_CAPTION "<multiple selection>"

// path definition
#define _server_root_       "$server_root$"
#define _server_data_root_  "$server_data_root$"
#define _local_root_        "$local_root$"
#define _import_            "$import$"
#define _sounds_            "$sounds$"
#define _textures_          "$textures$"
#define _objects_           "$objects$"
#define _maps_              "$maps$"
#define _groups_            "$groups$"
#define _temp_              "$temp$"
#define _omotion_           "$omotion$"
#define _omotions_          "$omotions$"
#define _smotion_           "$smotion$"
#define _detail_objects_    "$detail_objects$"

#define TEX_POINT_ATT       "internal\\internal_light_attpoint"
#define TEX_SPOT_ATT        "internal\\internal_light_attclip"

#include "../../xrRender/Private/ETextureParams.h"
#include "../../xrRender/Private/ResourceManager.h"
#include "Editor/ImageManager.h"

inline xr_string ChangeFileExt(const char* name, const char* e)
{
    xr_string path = name;

    LPCSTR    ext  = strext(name);
    if (ext)
    {
        if (int len = strlen(ext))
            return path.replace(path.length() - len, xr_string::npos, e);
    }
    else
        return path.append(e);
}

inline xr_string ChangeFileExt(const xr_string& name, const char* e)
{
    return ChangeFileExt(name.c_str(), e);
}
inline u32 TColor(u32 r)
{
    return r;
}

#ifdef XRECORE_EXPORTS
inline void not_implemented()
{
    if (IsDebuggerPresent())
        DebugBreak();
    else
    {
        R_ASSERT(0);
    }
}
#endif
