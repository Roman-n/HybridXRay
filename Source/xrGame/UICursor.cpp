#include "stdafx.h"
#include "uicursor.h"

#include "ui/UIStatic.h"
#include "UI3dStatic.h"
#include "ui/UIBtnHint.h"
#include "../xrRender/Public/KinematicsAnimated.h"

#define C_DEFAULT color_xrgb(0xff, 0xff, 0xff)

CUICursor::CUICursor(): m_static(NULL), m_3dstatic(NULL), m_b_use_win_cursor(false)
{
    bVisible = false;
    vPrevPos.set(0.0f, 0.0f);
    vPos.set(0.f, 0.f);
    InitInternal();
    Device->seqRender.Add(this, -3 /*2*/);
    Device->seqResolutionChanged.Add(this);
    m_3dstatic->SetVisual(rVisual_3dstatic);
}
//--------------------------------------------------------------------
CUICursor::~CUICursor()
{
    xr_delete(m_static);
    xr_delete(m_3dstatic);
    Device->seqRender.Remove(this);
    Device->seqResolutionChanged.Remove(this);
}

void CUICursor::OnScreenResolutionChanged()
{
    xr_delete(m_static);
    xr_delete(m_3dstatic);
    InitInternal();
}

void CUICursor::InitInternal()
{
    m_static = xr_new<CUIStatic>();
    m_3dstatic = xr_new<CUI3dStatic>();
    m_static->InitTextureEx("ui\\ui_ani_cursor", "hud\\cursor");
    const shared_str& visual_3dcursor = pSettings->r_string(UI3dCURSOR_SECT, "visual_3d");
    rVisual_3dstatic = smart_cast<IRenderVisual*>(Render->model_Create(visual_3dcursor.c_str()));
    Frect rect;
    rect.set(0.0f, 0.0f, 40.0f, 40.0f);
    m_static->SetTextureRect(rect);
    m_3dstatic->SetTextureRect(rect);
    Fvector2 sz;
    sz.set(rect.rb);
    sz.x *= UI().get_current_kx();

    m_static->SetWndSize(sz);
    m_static->SetStretchTexture(true);
    m_3dstatic->SetWndSize(sz);
    m_3dstatic->SetStretchTexture(true);

    u32 screen_size_x  = GetSystemMetrics(SM_CXSCREEN);
    u32 screen_size_y  = GetSystemMetrics(SM_CYSCREEN);
    m_b_use_win_cursor = (screen_size_y >= Device->dwHeight && screen_size_x >= Device->dwWidth);

    IKinematicsAnimated* V = smart_cast<IKinematicsAnimated*>(m_3dstatic->m_pCurrentItem = rVisual_3dstatic);
    if (V)
        V->PlayCycle("idle");
}

//--------------------------------------------------------------------
u32  last_render_frame = 0;
void CUICursor::OnRender()
{
    g_btnHint->OnRender();
    g_statHint->OnRender();

    if (!IsVisible())
        return;
#ifdef DEBUG
    VERIFY(last_render_frame != Device->dwFrame);
    last_render_frame = Device->dwFrame;

    if (bDebug)
    {
        CGameFont* F = UI().Font().pFontDI;
        F->SetAligment(CGameFont::alCenter);
        F->SetHeightI(0.02f);
        F->OutSetI(0.f, -0.9f);
        F->SetColor(0xffffffff);
        Fvector2 pt = GetCursorPosition();
        F->OutNext("%f-%f", pt.x, pt.y);
    }
#endif

        //if (!g_pGameLevel)
        {
            m_static->SetWndPos(vPos);
            m_static->Update();
            m_static->Draw();
        }
        //else
        {
            m_3dstatic->SetWndPos(vPos);
            m_3dstatic->Update();
            m_3dstatic->Draw();
        }
}

Fvector2 CUICursor::GetCursorPosition()
{
    return vPos;
}

Fvector2 CUICursor::GetCursorPositionDelta()
{
    Fvector2 res_delta;

    res_delta.x = vPos.x - vPrevPos.x;
    res_delta.y = vPos.y - vPrevPos.y;
    return res_delta;
}

void CUICursor::UpdateCursorPosition(int _dx, int _dy)
{
    Fvector2 p;
    vPrevPos = vPos;
    if (m_b_use_win_cursor)
    {
        POINT pti;
        BOOL  r = GetCursorPos(&pti);
        if (!r)
            return;
        p.x    = (float)pti.x;
        p.y    = (float)pti.y;
        vPos.x = p.x * (UI_BASE_WIDTH / (float)Device->dwWidth);
        vPos.y = p.y * (UI_BASE_HEIGHT / (float)Device->dwHeight);
    }
    else
    {
        float sens = 1.0f;
        vPos.x += _dx * sens;
        vPos.y += _dy * sens;
    }
    clamp(vPos.x, 0.f, UI_BASE_WIDTH);
    clamp(vPos.y, 0.f, UI_BASE_HEIGHT);
}

void CUICursor::SetUICursorPosition(Fvector2 pos)
{
    vPos = pos;
    POINT p;
    p.x = iFloor(vPos.x / (UI_BASE_WIDTH / (float)Device->dwWidth));
    p.y = iFloor(vPos.y / (UI_BASE_HEIGHT / (float)Device->dwHeight));

    SetCursorPos(p.x, p.y);
}
