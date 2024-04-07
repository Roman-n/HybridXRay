#include "stdafx.h"
#pragma hdrstop

#include "Environment.h"
#include "xr_efflensflare.h"
#include "rain.h"
#include "thunderbolt.h"

#if 1
// #include "render.h"
#include "igame_level.h"
#endif

//-----------------------------------------------------------------------------
// Environment render
//-----------------------------------------------------------------------------
extern ENGINE_API float psHUD_FOV;
// BOOL bNeed_re_create_env = FALSE;
void CEnvironment::RenderSky()
{
    if (g_pGameLevel)
    {
        if (0 == g_pGameLevel && !Device->IsEditorMode())
            return;
    }

    m_pRender->RenderSky(*this);
    /*
    // clouds_sh.create("clouds", "null");
    // this is the bug-fix for the case when the sky is broken
    // for some unknown reason the geoms happen to be invalid sometimes
    // if vTune show this in profile, please add simple cache (move-to-forward last found)
    // to the following functions:
    // CResourceManager::_CreateDecl
    // CResourceManager::CreateGeom
    if (bNeed_re_create_env)
    {
        sh_2sky.create(&m_b_skybox, "skybox_2t");
        sh_2geom.create(v_skybox_fvf, RCache.Vertex.Buffer(), RCache.Index.Buffer());
        clouds_sh.create("clouds", "null");
        clouds_geom.create(v_clouds_fvf, RCache.Vertex.Buffer(), RCache.Index.Buffer());
        bNeed_re_create_env = FALSE;
    }
    ::Render->rmFar();

    // draw sky box
    Fmatrix mSky;
    mSky.rotateY(CurrentEnv->sky_rotation);
    mSky.translate_over(Device->vCameraPosition);

    u32  i_offset, v_offset;
    u32  C   = color_rgba(iFloor(CurrentEnv->sky_color.x * 255.f), iFloor(CurrentEnv->sky_color.y * 255.f), iFloor(CurrentEnv->sky_color.z * 255.f), iFloor(CurrentEnv->weight * 255.f));

    // Fill index buffer
    u16* pib = RCache.Index.Lock(20 * 3, i_offset);
    CopyMemory(pib, hbox_faces, 20 * 3 * 2);
    RCache.Index.Unlock(20 * 3);

    // Fill vertex buffer
    v_skybox* pv = (v_skybox*)RCache.Vertex.Lock(12, sh_2geom.stride(), v_offset);
    for (u32 v = 0; v < 12; v++)
        pv[v].set(hbox_verts[v * 2], C, hbox_verts[v * 2 + 1]);
    RCache.Vertex.Unlock(12, sh_2geom.stride());

    // Render
    RCache.set_xform_world(mSky);
    RCache.set_Geometry(sh_2geom);
    RCache.set_Shader(sh_2sky);
    RCache.set_Textures(&CurrentEnv->sky_r_textures);
    RCache.Render(D3DPT_TRIANGLELIST, v_offset, 0, 12, i_offset, 20);

    // Sun
    ::Render->rmNormal();
    eff_LensFlare->Render(TRUE, FALSE, FALSE);
    */
}

void CEnvironment::RenderClouds()
{
    if (g_pGameLevel)
    {
        if (0 == g_pGameLevel && !Device->IsEditorMode())
            return;
    }

    // draw clouds
    if (fis_zero(CurrentEnv->clouds_color.w, EPS_L))
        return;

    m_pRender->RenderClouds(*this);
}

void CEnvironment::RenderFlares()
{
    if (g_pGameLevel)
    {
        if (0 == g_pGameLevel)
            return;
    }
    // 1
    eff_LensFlare->Render(FALSE, TRUE, TRUE);
}

void CEnvironment::RenderLast()
{
    if (g_pGameLevel)
    {
        if (0 == g_pGameLevel && !Device->IsEditorMode())
            return;
    }

    // 2
    eff_Rain->Render();
    eff_Thunderbolt->Render();
}

void CEnvironment::OnDeviceCreate()
{
    // bNeed_re_create_env = TRUE;
    m_pRender->OnDeviceCreate();

    // weathers
    {
        EnvsMapIt _I, _E;
        _I = WeatherCycles.begin();
        _E = WeatherCycles.end();
        for (; _I != _E; _I++)
            for (EnvIt it = _I->second.begin(); it != _I->second.end(); it++)
                (*it)->on_device_create();
    }
    // effects
    {
        EnvsMapIt _I, _E;
        _I = WeatherFXs.begin();
        _E = WeatherFXs.end();
        for (; _I != _E; _I++)
            for (EnvIt it = _I->second.begin(); it != _I->second.end(); it++)
                (*it)->on_device_create();
    }

    Invalidate();
    // OnFrame();
}

void CEnvironment::OnDeviceDestroy()
{
    m_pRender->OnDeviceDestroy();

    // weathers
    {
        EnvsMapIt _I, _E;
        _I = WeatherCycles.begin();
        _E = WeatherCycles.end();
        for (; _I != _E; _I++)
            for (EnvIt it = _I->second.begin(); it != _I->second.end(); it++)
                (*it)->on_device_destroy();
    }
    // effects
    {
        EnvsMapIt _I, _E;
        _I = WeatherFXs.begin();
        _E = WeatherFXs.end();
        for (; _I != _E; _I++)
            for (EnvIt it = _I->second.begin(); it != _I->second.end(); it++)
                (*it)->on_device_destroy();
    }
    if (CurrentEnv)
        CurrentEnv->destroy();
}

void CEnvironment::ED_Reload()
{
    OnDeviceDestroy();
    OnDeviceCreate();
}
