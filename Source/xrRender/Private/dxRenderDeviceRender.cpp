#include "stdafx.h"
#include "dxRenderDeviceRender.h"

#include "ResourceManager.h"

dxRenderDeviceRender::dxRenderDeviceRender()
#ifndef REDITOR
    :
    Resources(0)
#endif
{
    ;
}

void dxRenderDeviceRender::Copy(IRenderDeviceRender& _in)
{
    *this = *(dxRenderDeviceRender*)&_in;
}

void dxRenderDeviceRender::setGamma(float fGamma)
{
#ifndef REDITOR
    m_Gamma.Gamma(fGamma);
#endif
}

void dxRenderDeviceRender::setBrightness(float fGamma)
{
#ifndef REDITOR
    m_Gamma.Brightness(fGamma);
#endif
}

void dxRenderDeviceRender::setContrast(float fGamma)
{
#ifndef REDITOR
    m_Gamma.Contrast(fGamma);
#endif
}

void dxRenderDeviceRender::updateGamma()
{
#ifndef REDITOR
    m_Gamma.Update();
#endif
}

void dxRenderDeviceRender::OnDeviceDestroy(BOOL bKeepTextures)
{
#ifndef REDITOR
    m_WireShader.destroy();
    m_SelectionShader.destroy();

    Resources->OnDeviceDestroy(bKeepTextures);
    RCache.OnDeviceDestroy();
#endif
}

void dxRenderDeviceRender::ValidateHW()
{
    HW.Validate();
}

void dxRenderDeviceRender::DestroyHW()
{
#ifndef REDITOR
    xr_delete(Resources);
    HW.DestroyDevice();
#endif
}

void dxRenderDeviceRender::Reset(HWND hWnd, u32& dwWidth, u32& dwHeight, float& fWidth_2, float& fHeight_2)
{
#ifndef REDITOR
#ifdef DEBUG
    _SHOW_REF("*ref -CRenderDevice::ResetTotal: DeviceREF:", HW.pDevice);
#endif   // DEBUG

    Resources->reset_begin();
    Memory.mem_compact();
    HW.Reset(hWnd);

#if defined(USE_DX10) || defined(USE_DX11)
    dwWidth  = HW.m_ChainDesc.BufferDesc.Width;
    dwHeight = HW.m_ChainDesc.BufferDesc.Height;
#else    //	USE_DX10
    dwWidth  = HW.DevPP.BackBufferWidth;
    dwHeight = HW.DevPP.BackBufferHeight;
#endif   //	USE_DX10

    fWidth_2  = float(dwWidth / 2);
    fHeight_2 = float(dwHeight / 2);
    Resources->reset_end();

#ifdef DEBUG
    _SHOW_REF("*ref +CRenderDevice::ResetTotal: DeviceREF:", HW.pDevice);
#endif   // DEBUG

#endif
}

void dxRenderDeviceRender::SetupStates()
{
#ifndef REDITOR
    HW.Caps.Update();

#if defined(USE_DX10) || defined(USE_DX11)
    //	TODO: DX10: Implement Resetting of render states into default mode
    // VERIFY(!"dxRenderDeviceRender::SetupStates not implemented.");
#else   //	USE_DX10
    for (u32 i = 0; i < HW.Caps.raster.dwStages; i++)
    {
        float fBias = -.5f;
        CHK_DX(HW.pDevice->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, 4));
        CHK_DX(HW.pDevice->SetSamplerState(i, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD)(&fBias))));
        CHK_DX(HW.pDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR));
        CHK_DX(HW.pDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR));
        CHK_DX(HW.pDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR));
    }
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_ZENABLE, TRUE));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_LOCALVIEWER, TRUE));

    CHK_DX(HW.pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_COLOR1));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE));

    if (psDeviceFlags.test(rsWireframe))
    {
        CHK_DX(HW.pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME));
    }
    else
    {
        CHK_DX(HW.pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID));
    }

    // ******************** Fog parameters
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_FOGCOLOR, 0));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE));
    if (HW.Caps.bTableFog)
    {
        CHK_DX(HW.pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR));
        CHK_DX(HW.pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE));
    }
    else
    {
        CHK_DX(HW.pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE));
        CHK_DX(HW.pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR));
    }

#endif   //	USE_DX10
#endif
}

void dxRenderDeviceRender::OnDeviceCreate(LPCSTR shName)
{
#ifndef REDITOR
    // Signal everyone - device created
    RCache.OnDeviceCreate();
    m_Gamma.Update();
    Resources->OnDeviceCreate(shName);
    ::Render->create();
    Device->Statistic->OnDeviceCreate();

    // #ifndef DEDICATED_SERVER
    if (!g_dedicated_server)
    {
        m_WireShader.create("editor\\wire");
        m_SelectionShader.create("editor\\selection");

        DUImpl.OnDeviceCreate();
    }
#endif
    // #endif
}

void dxRenderDeviceRender::Create(HWND hWnd, u32& dwWidth, u32& dwHeight, float& fWidth_2, float& fHeight_2, bool move_window)
{
#ifndef REDITOR
    HW.CreateDevice(hWnd, move_window);
#if defined(USE_DX10) || defined(USE_DX11)
    dwWidth  = HW.m_ChainDesc.BufferDesc.Width;
    dwHeight = HW.m_ChainDesc.BufferDesc.Height;
#else    //	USE_DX10
    dwWidth  = HW.DevPP.BackBufferWidth;
    dwHeight = HW.DevPP.BackBufferHeight;
#endif   //	USE_DX10
    fWidth_2  = float(dwWidth / 2);
    fHeight_2 = float(dwHeight / 2);
    Resources = xr_new<CResourceManager>();
#endif
}

void dxRenderDeviceRender::SetupGPU(BOOL bForceGPU_SW, BOOL bForceGPU_NonPure, BOOL bForceGPU_REF)
{
#ifndef REDITOR
    HW.Caps.bForceGPU_SW      = bForceGPU_SW;
    HW.Caps.bForceGPU_NonPure = bForceGPU_NonPure;
    HW.Caps.bForceGPU_REF     = bForceGPU_REF;
#endif
}

void dxRenderDeviceRender::overdrawBegin()
{
#ifndef REDITOR
#if defined(USE_DX10) || defined(USE_DX11)
    //	TODO: DX10: Implement overdrawBegin
    VERIFY(!"dxRenderDeviceRender::overdrawBegin not implemented.");
#else    //	USE_DX10
    // Turn stenciling
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_STENCILREF, 0));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_STENCILMASK, 0x00000000));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff));

    // Increment the stencil buffer for each pixel drawn
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT));

    if (1 == HW.Caps.SceneMode)
    {
        CHK_DX(HW.pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP));
    }   // Overdraw
    else
    {
        CHK_DX(HW.pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCRSAT));
    }   // ZB access
#endif   //	USE_DX10
#endif
}

void dxRenderDeviceRender::overdrawEnd()
{
#ifndef REDITOR
#if defined(USE_DX10) || defined(USE_DX11)
    //	TODO: DX10: Implement overdrawEnd
    VERIFY(!"dxRenderDeviceRender::overdrawBegin not implemented.");
#else    //	USE_DX10
    // Set up the stencil states
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL));
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff));

    // Set the background to black
    CHK_DX(HW.pDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 0, 0), 0, 0));

    // Draw a rectangle wherever the count equal I
    RCache.OnFrameEnd();
    CHK_DX(HW.pDevice->SetFVF(FVF::F_TL));

    // Render gradients
    for (int I = 0; I < 12; I++)
    {
        u32     _c = I * 256 / 13;
        u32     c  = D3DCOLOR_XRGB(_c, _c, _c);

        FVF::TL pv[4];
        pv[0].set(float(0), float(Device->dwHeight), c, 0, 0);
        pv[1].set(float(0), float(0), c, 0, 0);
        pv[2].set(float(Device->dwWidth), float(Device->dwHeight), c, 0, 0);
        pv[3].set(float(Device->dwWidth), float(0), c, 0, 0);

        CHK_DX(HW.pDevice->SetRenderState(D3DRS_STENCILREF, I));
        CHK_DX(HW.pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pv, sizeof(FVF::TL)));
    }
    CHK_DX(HW.pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE));
#endif   //	USE_DX10
#endif
}

void dxRenderDeviceRender::DeferredLoad(BOOL E)
{
#ifndef REDITOR
    Resources->DeferredLoad(E);
#endif
}

void dxRenderDeviceRender::ResourcesDeferredUpload()
{
#ifndef REDITOR
    Resources->DeferredUpload();
#endif
}

void dxRenderDeviceRender::ResourcesGetMemoryUsage(u32& m_base, u32& c_base, u32& m_lmaps, u32& c_lmaps)
{
#ifndef REDITOR
    if (Resources)
        Resources->_GetMemoryUsage(m_base, c_base, m_lmaps, c_lmaps);
#endif
}

void dxRenderDeviceRender::ResourcesStoreNecessaryTextures()
{
#ifndef REDITOR
    dxRenderDeviceRender::Instance().Resources->StoreNecessaryTextures();
#endif
}

void dxRenderDeviceRender::ResourcesDumpMemoryUsage()
{
#ifndef REDITOR
    dxRenderDeviceRender::Instance().Resources->_DumpMemoryUsage();
#endif
}

dxRenderDeviceRender::DeviceState dxRenderDeviceRender::GetDeviceState()
{
#ifndef REDITOR
    HW.Validate();
#if defined(USE_DX10) || defined(USE_DX11)
    //	TODO: DX10: Implement GetDeviceState
    //	TODO: DX10: Implement DXGI_PRESENT_TEST testing
    // VERIFY(!"dxRenderDeviceRender::overdrawBegin not implemented.");
#else    //	USE_DX10
    HRESULT _hr = HW.pDevice->TestCooperativeLevel();
    if (FAILED(_hr))
    {
        // If the device was lost, do not render until we get it back
        if (D3DERR_DEVICELOST == _hr)
            return dsLost;

        // Check if the device is ready to be reset
        if (D3DERR_DEVICENOTRESET == _hr)
            return dsNeedReset;
    }
#endif   //	USE_DX10

    return dsOK;
#endif
    return dsOK;
}

BOOL dxRenderDeviceRender::GetForceGPU_REF()
{
#ifndef REDITOR
    return HW.Caps.bForceGPU_REF;
#endif
    return FALSE;
}

u32 dxRenderDeviceRender::GetCacheStatPolys()
{
#ifndef REDITOR
    return RCache.stat.polys;
#endif
    return 0;
}

void dxRenderDeviceRender::Begin()
{
#ifndef REDITOR
#if !defined(USE_DX10) && !defined(USE_DX11)
    CHK_DX(HW.pDevice->BeginScene());
#endif   //	USE_DX10
    RCache.OnFrameBegin();
    RCache.set_CullMode(CULL_CW);
    RCache.set_CullMode(CULL_CCW);
    if (HW.Caps.SceneMode)
        overdrawBegin();
#endif
}

void dxRenderDeviceRender::Clear()
{
#ifndef REDITOR
#if defined(USE_DX10) || defined(USE_DX11)
    HW.pContext->ClearDepthStencilView(RCache.get_ZB(), D3D_CLEAR_DEPTH | D3D_CLEAR_STENCIL, 1.0f, 0);

    if (psDeviceFlags.test(rsClearBB))
    {
        FLOAT ColorRGBA[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        HW.pContext->ClearRenderTargetView(RCache.get_RT(), ColorRGBA);
    }
#else    //	USE_DX10
    CHK_DX(HW.pDevice->Clear(0, 0, D3DCLEAR_ZBUFFER | (psDeviceFlags.test(rsClearBB) ? D3DCLEAR_TARGET : 0) | (HW.Caps.bStencil ? D3DCLEAR_STENCIL : 0), D3DCOLOR_XRGB(0, 0, 0), 1, 0));
#endif   //	USE_DX10
#endif
}

void DoAsyncScreenshot();

void dxRenderDeviceRender::End()
{
#ifndef REDITOR
    VERIFY(HW.pDevice);

    if (HW.Caps.SceneMode)
        overdrawEnd();

    RCache.OnFrameEnd();
    Memory.dbg_check();

    DoAsyncScreenshot();

#if defined(USE_DX10) || defined(USE_DX11)
    HW.m_pSwapChain->Present(0, 0);
#else    //	USE_DX10
    CHK_DX(HW.pDevice->EndScene());

    HW.pDevice->Present(NULL, NULL, NULL, NULL);
#endif   //	USE_DX10 \
         // HRESULT _hr		= HW.pDevice->Present( NULL, NULL, NULL, NULL );
    // if				(D3DERR_DEVICELOST==_hr)	return;			// we will handle this later
#endif
}

void dxRenderDeviceRender::ResourcesDestroyNecessaryTextures()
{
#ifndef REDITOR
    Resources->DestroyNecessaryTextures();
#endif
}

void dxRenderDeviceRender::ClearTarget()
{
#ifndef REDITOR
#if defined(USE_DX10) || defined(USE_DX11)
    FLOAT ColorRGBA[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    HW.pContext->ClearRenderTargetView(RCache.get_RT(), ColorRGBA);
#else    //	USE_DX10
    CHK_DX(HW.pDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1, 0));
#endif   //	USE_DX10
#endif
}

void dxRenderDeviceRender::SetCacheXform(Fmatrix& mView, Fmatrix& mProject)
{
#ifndef REDITOR
    RCache.set_xform_view(mView);
    RCache.set_xform_project(mProject);
#endif
}

bool dxRenderDeviceRender::HWSupportsShaderYUV2RGB()
{
#ifndef REDITOR
    u32 v_dev  = CAP_VERSION(HW.Caps.raster_major, HW.Caps.raster_minor);
    u32 v_need = CAP_VERSION(2, 0);
    return (v_dev >= v_need);
#endif
    return false;
}

void dxRenderDeviceRender::OnAssetsChanged()
{
#ifndef REDITOR
    Resources->m_textures_description.UnLoad();
    Resources->m_textures_description.Load();
#endif
}
