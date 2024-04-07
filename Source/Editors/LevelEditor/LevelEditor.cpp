// LevelEditor.cpp : Определяет точку входа для приложения.
//
#include "stdafx.h"
#include "../../xrAPI\xrGameManager.h"
#include "Engine/xrGameManager.h"
#include "../xrEngine/std_classes.h"
#include "../xrEngine/IGame_Persistent.h"
#include "../xrEngine/XR_IOConsole.h"
#include "../xrEngine/IGame_Level.h"
#include "../xrEngine/x_ray.h"
#include "Engine/XRayEditor.h"
#include "resources/splash.h"

XREPROPS_API extern bool bIsActorEditor;
ECORE_API extern bool    bIsLevelEditor;
ECORE_API extern bool    bIsParticleEditor;
ECORE_API extern bool    bIsShaderEditor;

int WINAPI               wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    bIsActorEditor    = false;
    bIsLevelEditor    = true;
    bIsParticleEditor = false;
    bIsShaderEditor   = false;

    if (strstr(GetCommandLine(), "-nosplash") == nullptr)
    {
        constexpr bool topmost = false;
        splash::show(topmost);
    }
    splash::update_progress(1);

    if (!IsDebuggerPresent())
        Debug._initialize(false);

    splash::update_progress(5);
    const char* FSName = "fs.ltx";
    {
        if (xrGameManager::GetGame() == EGame::SHOC)
        {
            FSName = "fs_soc.ltx";
            Core._initialize("Level_Editor_ShoC", ELogCallback, 1, FSName, true);
        }
        else if (xrGameManager::GetGame() == EGame::CS)
        {
            FSName = "fs_cs.ltx";
            Core._initialize("Level_Editor_CS", ELogCallback, 1, FSName, true);
        }
        else
            Core._initialize("Level_Editor_CoP", ELogCallback, 1, FSName, true);
    }

    splash::update_progress(24);
    LTools = xr_new<CLevelTool>();
    Tools  = LTools;

    splash::update_progress(5);
    LUI = xr_new<CLevelMain>();
    UI  = LUI;
    UI->RegisterCommands();

    splash::update_progress(15);
    Scene                = xr_new<EScene>();
    EditorScene          = Scene;
    UIMainForm* MainForm = xr_new<UIMainForm>();
    pApp                 = xr_new<XRayEditor>();
    g_xrGameManager      = xr_new<xrGameManagerLE>();
    g_SEFactoryManager   = xr_new<xrSEFactoryManager>();

    splash::update_progress(24);
    g_pGamePersistent = (IGame_Persistent*)g_xrGameManager->Create(CLSID_GAME_PERSISTANT);
    EDevice->seqAppStart.Process(rp_AppStart);
    Console->Execute("default_controls");
    Console->Hide();

    ::MainForm = MainForm;
    UI->Push(MainForm, false);
    splash::update_progress(25);

    splash::update_progress(1);
    while (MainForm->Frame())
    {}

    xr_delete(MainForm);
    xr_delete(pApp);
    xr_delete(g_xrGameManager);
    xr_delete(g_SEFactoryManager);

    Core._destroy();
    splash::hide();
    return 0;
}
