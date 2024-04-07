// EngineAPI.cpp: implementation of the CEngineAPI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EngineAPI.h"
#include "../xrcdb/xrXRC.h"
#include "../xrAPI/xrGameManager.h"
#include "securom_api.h"
#include "device.h"
extern xr_token* vid_quality_token;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __cdecl dummy(void){};
CEngineAPI::CEngineAPI()
{
#ifndef SHIPPING
    hGame   = 0;
    hRender = 0;
    hTuner  = 0;
#endif
    pCreate     = 0;
    pDestroy    = 0;
    tune_pause  = dummy;
    tune_resume = dummy;
}

CEngineAPI::~CEngineAPI()
{
    // destroy quality token here
    if (vid_quality_token)
    {
        for (int i = 0; vid_quality_token[i].name; i++)
        {
            xr_free(vid_quality_token[i].name);
        }
        xr_free(vid_quality_token);
        vid_quality_token = NULL;
    }
}

extern u32      renderer_value;   // con cmd
ENGINE_API int  g_current_renderer = 0;

ENGINE_API bool is_enough_address_space_available()
{
    SYSTEM_INFO system_info;

    SECUROM_MARKER_HIGH_SECURITY_ON(12)

    GetSystemInfo(&system_info);

    SECUROM_MARKER_HIGH_SECURITY_OFF(12)

    return (*(u32*)&system_info.lpMaximumApplicationAddress) > 0x90000000;
}

#ifndef DEDICATED_SERVER

void CEngineAPI::InitializeNotDedicated()
{
#ifndef SHIPPING
    LPCSTR r2_name = "xrRender_R2.dll";
    LPCSTR r3_name = "xrRender_R3.dll";
    LPCSTR r4_name = "xrRender_R4.dll";
    LPCSTR r5_name = "xrRender_R5.dll";
    if (psDeviceFlags.test(rsR5))
    {
        Log("Loading DLL:", r5_name);
        hRender = LoadLibrary(r5_name);
        if (0 == hRender)
        {
            // try to load R1
            Msg("! ...Failed - incompatible hardware/pre-Vista OS.");
            psDeviceFlags.set(rsR2, TRUE);
        }
    }
    if (psDeviceFlags.test(rsR4))
    {
        // try to initialize R4
        Log("Loading DLL:", r4_name);
        hRender = LoadLibrary(r4_name);
        if (0 == hRender)
        {
            // try to load R1
            Msg("! ...Failed - incompatible hardware/pre-Vista OS.");
            psDeviceFlags.set(rsR2, TRUE);
        }
    }

    if (psDeviceFlags.test(rsR3))
    {
        // try to initialize R3
        Log("Loading DLL:", r3_name);
        hRender = LoadLibrary(r3_name);
        if (0 == hRender)
        {
            // try to load R1
            Msg("! ...Failed - incompatible hardware/pre-Vista OS.");
            psDeviceFlags.set(rsR2, TRUE);
        }
        else
            g_current_renderer = 3;
    }

    if (psDeviceFlags.test(rsR2))
    {
        // try to initialize R2
        psDeviceFlags.set(rsR5, FALSE);
        psDeviceFlags.set(rsR4, FALSE);
        psDeviceFlags.set(rsR3, FALSE);

        Log("Loading DLL:", r2_name);
        hRender = LoadLibrary(r2_name);
        if (0 == hRender)
        {
            // try to load R1
            Msg("! ...Failed - incompatible hardware.");
        }
        else
            g_current_renderer = 2;
    }

    SECUROM_MARKER_HIGH_SECURITY_OFF(2)
#else
    g_current_renderer = 2;
#endif
}
#endif   // DEDICATED_SERVER

#ifdef SHIPPING

extern "C" DLL_Pure* __cdecl xrFactory_Create(CLASS_ID clsid);
extern "C" void __cdecl xrFactory_Destroy(DLL_Pure* O);
extern "C" bool SupportsRendering();
extern "C" void InitializeRendering();
#else
extern "C"
{
    typedef bool SupportsRendering();
    typedef void InitializeRendering();
    typedef void xrGameInitialize();
};
#endif
void CEngineAPI::Initialize(void)
{
    //////////////////////////////////////////////////////////////////////////
    // render
#ifdef SHIPPING
    // InitializeNotDedicated();
    InitializeRendering();
    pCreate  = &xrFactory_Create;
    pDestroy = &xrFactory_Destroy;

#else
    LPCSTR r1_name = "xrRender_R1.dll";

#ifndef DEDICATED_SERVER
    InitializeNotDedicated();
#endif   // DEDICATED_SERVER

    if (0 == hRender)
    {
        // try to load R1
        psDeviceFlags.set(rsR4, FALSE);
        psDeviceFlags.set(rsR3, FALSE);
        psDeviceFlags.set(rsR2, FALSE);
        renderer_value = 0;   // con cmd

        Log("Loading DLL:", r1_name);
        hRender = LoadLibrary(r1_name);
        if (0 == hRender)
            Msg("Loading Error:%x", (u32)GetLastError());
        R_ASSERT(hRender);
        g_current_renderer = 1;
    }
    {
        InitializeRendering* pInitializeRendering = (InitializeRendering*)GetProcAddress(hRender, "InitializeRendering");
        R_ASSERT(pInitializeRendering);
        pInitializeRendering();
    }

    EngineDevice->ConnectToRender();
    // game
    {
        LPCSTR g_name = "xrGame.dll";
        switch (xrGameManager::GetGame())
        {
            case EGame::CS:
                g_name = "xrGameCS.dll";
                break;
            case EGame::SHOC:
                g_name = "xrGameSOC.dll";
                break;
        }

        Log("Loading DLL:", g_name);
        hGame = LoadLibrary(g_name);
        if (0 == hGame)
            R_CHK(GetLastError());
        R_ASSERT2(hGame, "Game DLL raised exception during loading or there is no game DLL at all");
        pCreate = (Factory_Create*)GetProcAddress(hGame, "xrFactory_Create");
        R_ASSERT(pCreate);
        pDestroy = (Factory_Destroy*)GetProcAddress(hGame, "xrFactory_Destroy");
        R_ASSERT(pDestroy);
        xrGameInitialize* pxrGameInitialize = (xrGameInitialize*)GetProcAddress(hGame, "xrGameInitialize");
        R_ASSERT(pxrGameInitialize);
        pxrGameInitialize();
    }

    //////////////////////////////////////////////////////////////////////////
    // vTune
    tune_enabled = FALSE;
    if (strstr(Core.Params, "-tune"))
    {
        LPCSTR g_name = "vTuneAPI.dll";
        Log("Loading DLL:", g_name);
        hTuner = LoadLibrary(g_name);
        if (0 == hTuner)
            R_CHK(GetLastError());
        R_ASSERT2(hTuner, "Intel vTune is not installed");
        tune_enabled = TRUE;
        tune_pause   = (VTPause*)GetProcAddress(hTuner, "VTPause");
        R_ASSERT(tune_pause);
        tune_resume = (VTResume*)GetProcAddress(hTuner, "VTResume");
        R_ASSERT(tune_resume);
    }
#endif
}

void CEngineAPI::Destroy(void)
{
#ifndef SHIPPING
    if (hGame)
    {
        FreeLibrary(hGame);
        hGame = 0;
    }
    if (hRender)
    {
        FreeLibrary(hRender);
        hRender = 0;
    }
#endif
    pCreate  = 0;
    pDestroy = 0;
    Engine.Event._destroy();
    XRC.r_clear_compact();
}

void CEngineAPI::CreateRendererList()
{
#ifndef SHIPPING
    //	TODO: ask renderers if they are supported!
    if (vid_quality_token != NULL)
        return;
    bool   bSupports_r2   = false;
    bool   bSupports_r2_5 = false;
    bool   bSupports_r3   = false;
    bool   bSupports_r4   = false;
    bool   bSupports_r5   = false;
    LPCSTR r2_name        = "xrRender_R2.dll";
    LPCSTR r3_name        = "xrRender_R3.dll";
    LPCSTR r4_name        = "xrRender_R4.dll";
    LPCSTR r5_name        = "xrRender_R5.dll";

    if (strstr(Core.Params, "-perfhud_hack"))
    {
        bSupports_r2   = true;
        bSupports_r2_5 = true;
        bSupports_r3   = true;
        bSupports_r4   = true;
        bSupports_r5   = true;
    }
    else
    {
        // try to initialize R2
        Log("Loading DLL:", r2_name);
        hRender = LoadLibrary(r2_name);
        if (hRender)
        {
            bSupports_r2                      = true;
            SupportsRendering* test_rendering = (SupportsRendering*)GetProcAddress(hRender, "SupportsRendering");
            R_ASSERT(test_rendering);
            bSupports_r2_5 = test_rendering();
            FreeLibrary(hRender);
        }

        // try to initialize R3
        Log("Loading DLL:", r3_name);
        //	Hide "d3d10.dll not found" message box for XP
        SetErrorMode(SEM_FAILCRITICALERRORS);
        hRender = LoadLibrary(r3_name);
        //	Restore error handling
        SetErrorMode(0);
        if (hRender)
        {
            SupportsRendering* test_dx10_rendering = (SupportsRendering*)GetProcAddress(hRender, "SupportsRendering");
            R_ASSERT(test_dx10_rendering);
            bSupports_r3 = test_dx10_rendering();
            FreeLibrary(hRender);
        }

        // try to initialize R4
        Log("Loading DLL:", r4_name);
        //	Hide "d3d10.dll not found" message box for XP
        SetErrorMode(SEM_FAILCRITICALERRORS);
        hRender = LoadLibrary(r4_name);
        //	Restore error handling
        SetErrorMode(0);
        if (hRender)
        {
            SupportsRendering* test_dx11_rendering = (SupportsRendering*)GetProcAddress(hRender, "SupportsRendering");
            R_ASSERT(test_dx11_rendering);
            bSupports_r4 = true;   // test_dx11_rendering();
            FreeLibrary(hRender);
        }

        hRender = LoadLibrary(r5_name);
        //	Restore error handling
        SetErrorMode(0);
        if (hRender)
        {
            SupportsRendering* test_dx11_rendering = (SupportsRendering*)GetProcAddress(hRender, "SupportsRendering");
            R_ASSERT(test_dx11_rendering);
            bSupports_r5 = test_dx11_rendering();
            FreeLibrary(hRender);
        }
    }

    hRender = 0;

    xr_vector<LPCSTR> _tmp;
    u32               i          = 0;
    bool              bBreakLoop = false;
    for (; i < 7; ++i)
    {
        switch (i)
        {
            case 1:
                if (!bSupports_r2)
                    bBreakLoop = true;
                break;
            case 3:   // "renderer_r2.5"
                if (!bSupports_r2_5)
                    bBreakLoop = true;
                break;
            case 4:   // "renderer_r_dx10"
                      // if (xrGameManager::GetGame() != EGame::COP)
                      //         bBreakLoop = true;
                      //     break;
                if (!bSupports_r3)
                    bBreakLoop = true;
                break;
            case 5:   // "renderer_r_dx11"
                if (!bSupports_r4)
                    bBreakLoop = true;
                break;
            case 6:   // "renderer_r_dx11"
                if (!bSupports_r5)
                    bBreakLoop = true;
                break;
            default:;
        }

        if (bBreakLoop)
            break;

        _tmp.push_back(NULL);
        LPCSTR val = NULL;
        switch (i)
        {
            case 0:
                val = "renderer_r1";
                break;
            case 1:
                val = "renderer_r2a";
                break;
            case 2:
                val = "renderer_r2";
                break;
            case 3:
                val = "renderer_r2.5";
                break;
            case 4:
                val = "renderer_r3";
                break;   //  -)
            case 5:
                val = "renderer_r4";
                break;   //  -)
            case 6:
                val = "renderer_r5";
                break;   //  -)
        }
        if (bBreakLoop)
            break;
        _tmp.back() = xr_strdup(val);
    }
    u32 _cnt                         = _tmp.size() + 1;
    vid_quality_token                = xr_alloc<xr_token>(_cnt);

    vid_quality_token[_cnt - 1].id   = -1;
    vid_quality_token[_cnt - 1].name = NULL;

#ifdef DEBUG
    Msg("Available render modes[%d]:", _tmp.size());
#endif   // DEBUG
    for (u32 i = 0; i < _tmp.size(); ++i)
    {
        vid_quality_token[i].id   = i;
        vid_quality_token[i].name = _tmp[i];
#ifdef DEBUG
        Msg("[%s]", _tmp[i]);
#endif   // DEBUG
    }

    /*
    if(vid_quality_token != NULL)		return;

    D3DCAPS9					caps;
    CHW							_HW;
    _HW.CreateD3D				();
    _HW.pD3D->GetDeviceCaps		(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&caps);
    _HW.DestroyD3D				();
    u16		ps_ver_major		= u16 ( u32(u32(caps.PixelShaderVersion)&u32(0xf << 8ul))>>8 );

    xr_vector<LPCSTR>			_tmp;
    u32 i						= 0;
    for(; i<5; ++i)
    {
        bool bBreakLoop = false;
        switch (i)
        {
        case 3:		//"renderer_r2.5"
            if (ps_ver_major < 3)
                bBreakLoop = true;
            break;
        case 4:		//"renderer_r_dx10"
            bBreakLoop = true;
            break;
        default:	;
        }

        if (bBreakLoop) break;

        _tmp.push_back				(NULL);
        LPCSTR val					= NULL;
        switch (i)
        {
        case 0: val ="renderer_r1";			break;
        case 1: val ="renderer_r2a";		break;
        case 2: val ="renderer_r2";			break;
        case 3: val ="renderer_r2.5";		break;
        case 4: val ="renderer_r_dx10";		break; //  -)
        }
        _tmp.back()					= xr_strdup(val);
    }
    u32 _cnt								= _tmp.size()+1;
    vid_quality_token						= xr_alloc<xr_token>(_cnt);

    vid_quality_token[_cnt-1].id			= -1;
    vid_quality_token[_cnt-1].name			= NULL;

#ifdef DEBUG
    Msg("Available render modes[%d]:",_tmp.size());
#endif // DEBUG
    for(u32 i=0; i<_tmp.size();++i)
    {
        vid_quality_token[i].id				= i;
        vid_quality_token[i].name			= _tmp[i];
#ifdef DEBUG
        Msg							("[%s]",_tmp[i]);
#endif // DEBUG
    }
    */
#endif
}
