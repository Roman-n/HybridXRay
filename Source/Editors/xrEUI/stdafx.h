#pragma once
#include "../../xrCore/xrCore.h"
#ifdef XREUI_EXPORTS
#define XREUI_API __declspec(dllexport)
#else
#define XREUI_API __declspec(dllimport)
#endif
#include "xrUI.h"
#include <d3d9.h>
#include "xrUIManager.h"

#define IMGUI_API XREUI_API
#define IMGUI_IMPL_API
using ImTextureID = IDirect3DBaseTexture9*;
#define ImTextureID ImTextureID
#define IMGUI_DISABLE_INCLUDE_IMCONFIG_H
#define IMGUI_INCLUDE_IMGUI_USER_H

#include "imgui.h"
#include "localization_types.h"

extern XREUI_API ELocalization EditorLocalization;
extern XREUI_API bool          bDemoImGui;

inline SEnglishStringToRussify operator"" _RU(const char* _Source, size_t)
{
    return SEnglishStringToRussify{_Source};
}

inline const char* operator>>(const SEnglishStringToRussify& _EnglishStringToRussify, const char* _ItsRussification)
{
    return EditorLocalization == ELocalization::EN ? _EnglishStringToRussify.String : _ItsRussification;
}

inline SLocalizedString operator>(const SEnglishStringToRussify& _EnglishStringToRussify, const char* _ItsRussification)
{
    return {_EnglishStringToRussify.String, _ItsRussification};
}
