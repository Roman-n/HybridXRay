#include "StdAfx.h"
#include "xrLevelEditor.h"
#include "Level_Bullet_Manager.h"
#include "xrserver.h"
#include "game_cl_base.h"
#include "xrmessages.h"
#include "xrGameSpyServer.h"
#include "../xrEngine/x_ray.h"
#include "../xrEngine/device.h"
#include "../xrEngine/IGame_Persistent.h"
#include "../xrEngine/xr_ioconsole.h"
#include "MainMenu.h"
#include "PHdynamicdata.h"
#include "Physics.h"
#include "../xrEngine/xrEditorSceneInterface.h"
#include "hudmanager.h"
extern BOOL               g_start_total_res;
extern xrServer::EConnect g_connect_server_err;
CLevelEditor::CLevelEditor() {}

CLevelEditor::~CLevelEditor() {}
extern pureFrame* g_pNetProcessor;
static void       build_callback(Fvector* V, int Vcnt, CDB::TRI* T, int Tcnt, void* params)
{
    g_pGameLevel->Load_GameSpecific_CFORM(T, Tcnt);
}
void LoadPhysicsGameParams();
BOOL CLevelEditor::net_Start(LPCSTR op_server, LPCSTR op_client)
{
    net_start_result_total = TRUE;
    //make Client Name if options doesn't have it
    m_caServerOptions      = op_server;
    //---------------------------------------------------------------------
    m_bDemoPlayMode        = FALSE;
    m_aDemoData.clear();
    m_bDemoStarted    = FALSE;
    //---------------------------------------------------------------------------

    Server            = xr_new<xrServer>();
    m_name            = "test";
    m_caServerOptions = op_server;
    m_caClientOptions = op_client;
    auto& p           = g_pGamePersistent->m_game_params;
    xr_strcpy(p.m_game_type, "single");
    xr_strcpy(p.m_alife, "alife");
    xr_strcpy(p.m_new_or_load, "editor");
    xr_strcpy(p.m_game_or_spawn, "editor");
    p.m_e_game_type = eGameIDSingle;

    if ((Server->Connect(m_caServerOptions)) != xrServer::ErrNoError)
    {
        Msg("! Failed to start server.");
        return false;
    }
    Server->SLS_Default();
    m_name = Server->level_name(m_caServerOptions);

    if (psNET_direct_connect)
    {
        Server->create_direct_client();
        //offline account creation
    }

    connected_to_server = Connect2Server(*m_caClientOptions);

    if (!connected_to_server)
        return FALSE;

    {
        LPCSTR level_name   = NULL;
        LPCSTR level_ver    = NULL;
        LPCSTR download_url = NULL;

        if (psNET_direct_connect)   //single
        {
            shared_str const& server_options = Server->GetConnectOptions();
            level_name                       = name().c_str();   //Server->level_name		(server_options).c_str();
            level_ver                        = "1.0";
        }
        else   //multiplayer
        {
            R_ASSERT(false);
        }

#ifdef DEBUG
        Msg("--- net_start_client3: level_id [%d], level_name[%s], level_version[%s]", 0, level_name, level_ver);
#endif   // #ifdef DEBUG \
    // Begin spawn

        // Send physics to single or multithreaded mode
        LoadPhysicsGameParams();
        ph_world = xr_new<CPHWorld>();

        // Send network to single or multithreaded mode
        // *note: release version always has "mt_*" enabled
        Device->seqFrameMT.Remove(g_pNetProcessor);
        Device->seqFrame.Remove(g_pNetProcessor);
        if (psDeviceFlags.test(mtNetwork))
            Device->seqFrameMT.Add(g_pNetProcessor, REG_PRIORITY_HIGH + 2);
        else
            Device->seqFrame.Add(g_pNetProcessor, REG_PRIORITY_LOW - 2);

        if (!psNET_direct_connect)
        {
            // Waiting for connection/configuration completition
            CTimer timer_sync;
            timer_sync.Start();
            while (!net_isCompleted_Connect())
                Sleep(5);
            Msg("* connection sync: %d ms", timer_sync.GetElapsed_ms());
            while (!net_isCompleted_Sync())
            {
                ClientReceive();
                Sleep(5);
            }
        }
        {
            IReader F(nullptr, 0, 0);
            pLevel = xr_new<CInifile>(&F);
        }
        {
            Device->seqRender.Add(this);
            Device->seqFrame.Add(this);
            R_ASSERT(Load_GameSpecific_Before());
            Objects.Load();
            EditorScene->LoadCFrom(&ObjectSpace, build_callback);
            ph_world->Create();
            R_ASSERT(Load_GameSpecific_After());
            bReady = true;
        }

        while (!game_configured)
        {
            ClientReceive();
            if (Server)
                Server->Update();
            Sleep(5);
        }

        if (!g_dedicated_server)
        {
            HUD().Load();
        }
        // Sync
        if (g_hud)
            g_hud->OnConnected();

        g_pGamePersistent->LoadTitle("st_client_synchronising");
        //Device->PreCache(30, true, true);
    }
    {
        NET_Packet NP;
        NP.w_begin(M_CLIENTREADY);
        Send(NP, net_flags(TRUE, TRUE));

        if (OnClient() && Server)
        {
            Server->SLS_Clear();
        };
    };

    g_start_total_res = net_start_result_total;

    //init bullet manager
    BulletManager().Clear();
    BulletManager().Load();

    {
        if (strstr(Core.Params, "-$"))
        {
            string256 buf, cmd, param;
            sscanf(strstr(Core.Params, "-$") + 2, "%[^ ] %[^ ] ", cmd, param);
            strconcat(sizeof(buf), buf, cmd, " ", param);
            Console->Execute(buf);
        }

        if (!g_dedicated_server)
        {
            if (g_hud)
                HUD().GetUI()->OnConnected();
        }
    }

    return TRUE;
}
