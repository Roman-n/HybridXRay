// ParticleEditor.cpp : Определяет точку входа для приложения.
//
#include "stdafx.h"
#include "resources\splash.h"

XREPROPS_API extern bool bIsActorEditor;
ECORE_API extern bool    bIsLevelEditor;
ECORE_API extern bool    bIsParticleEditor;
ECORE_API extern bool    bIsShaderEditor;

int WINAPI               wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    bIsActorEditor    = false;
    bIsLevelEditor    = false;
    bIsParticleEditor = true;
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
            Core._initialize("Patricle_Editor_ShoC", ELogCallback, 1, FSName, true);
        }
        else if (xrGameManager::GetGame() == EGame::CS)
        {
            FSName = "fs_cs.ltx";
            Core._initialize("Patricle_Editor_CS", ELogCallback, 1, FSName, true);
        }
        else
            Core._initialize("Patricle_Editor_CoP", ELogCallback, 1, FSName, true);
    }

    splash::update_progress(39);
    PTools = xr_new<CParticleTool>();
    Tools  = PTools;

    splash::update_progress(5);
    UI = xr_new<CParticleMain>();
    UI->RegisterCommands();

    splash::update_progress(20);
    UIMainForm* MainForm = xr_new<UIMainForm>();
    ::MainForm           = MainForm;
    UI->Push(MainForm, false);
    splash::update_progress(29);

    splash::update_progress(1);
    while (MainForm->Frame())
    {}

    xr_delete(MainForm);
    Core._destroy();
    splash::hide();
    return 0;
}
