#ifndef xrApi_included
#define xrApi_included
#pragma once
#ifdef SHIPPING
#define XRAPI_API
#else
#ifdef XRAPI_EXPORTS
#define XRAPI_API __declspec(dllexport)
#else
#define XRAPI_API __declspec(dllimport)
#endif
#endif

class xrDeviceInterface;
extern XRAPI_API xrDeviceInterface* Device;

class IRender_interface;
extern XRAPI_API IRender_interface* Render;

class IRenderFactory;
extern XRAPI_API IRenderFactory* RenderFactory;

class CDUInterface;
extern XRAPI_API CDUInterface* DU;

struct xr_token;
extern XRAPI_API xr_token* vid_mode_token;

class IUIRender;
extern XRAPI_API IUIRender* UIRender;

extern XRAPI_API bool       g_dedicated_server;
class IGame_Persistent;
extern XRAPI_API IGame_Persistent* g_pGamePersistent;

class xrGameMaterialLibraryInterface;
extern XRAPI_API xrGameMaterialLibraryInterface* GameMaterialLibrary;

#ifndef SHIPPING
class xrGameEditorInterface;
extern XRAPI_API xrGameEditorInterface* GameEditor;
class xrEditorSceneInterface;
extern XRAPI_API xrEditorSceneInterface* EditorScene;
#endif

#ifdef DEBUG
class IDebugRender;
extern XRAPI_API IDebugRender* DRender;
#endif   // DEBUG

/*
// This class is exported from the xrAPI.dll
class XRAPI_API CxrAPI {
public:
    CxrAPI(void);
    // TODO: add your methods here.
};

extern XRAPI_API int nxrAPI;

XRAPI_API int fnxrAPI(void);
*/

#endif   //	xrApi_included
