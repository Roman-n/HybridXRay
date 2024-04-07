#include "stdafx.h"

#include "IGame_Persistent.h"
#include "../xrAPI/xrGameManager.h"
#include "xrGamePersistentEditors.h"

xrGamePersistentEditors::xrGamePersistentEditors(): IGame_Persistent(true)
{
    g_dedicated_server = false;
    m_pMainMenu        = NULL;
    pEnvironment       = nullptr;
    pEnvironment       = xr_new<CEnvironment>();
}

xrGamePersistentEditors::~xrGamePersistentEditors() {}

void xrGamePersistentEditors::OnAppActivate() {}

void xrGamePersistentEditors::OnAppDeactivate() {}

void xrGamePersistentEditors::OnAppStart() {}

void xrGamePersistentEditors::OnAppEnd()
{
    OnGameEnd();
}

void xrGamePersistentEditors::PreStart(LPCSTR op)
{
    string256 prev_type;
    params    new_game_params;
    xr_strcpy(prev_type, m_game_params.m_game_type);
    new_game_params.parse_cmd_line(op);

    // change game type
    if (0 != xr_strcmp(prev_type, new_game_params.m_game_type))
    {
        OnGameEnd();
    }
}
void xrGamePersistentEditors::Start(LPCSTR op)
{
    string256 prev_type;
    xr_strcpy(prev_type, m_game_params.m_game_type);
    m_game_params.parse_cmd_line(op);
    // change game type
    if ((0 != xr_strcmp(prev_type, m_game_params.m_game_type)))
    {
        if (*m_game_params.m_game_type)
            OnGameStart();
    }
    else
        UpdateGameType();

    VERIFY(ps_destroy.empty());
}

void  xrGamePersistentEditors::Disconnect() {}

void  xrGamePersistentEditors::OnGameStart() {}

void  xrGamePersistentEditors::OnGameEnd() {}

void  xrGamePersistentEditors::OnFrame() {}

void  xrGamePersistentEditors::OnAssetsChanged() {}

void  xrGamePersistentEditors::RegisterModel(IRenderVisual* V) {}

float xrGamePersistentEditors::MtlTransparent(u32 mtl_idx)
{
    return 0.0f;
}

void xrGamePersistentEditors::Statistics(CGameFont* F) {}
