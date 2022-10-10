﻿// LevelEditor.cpp : Определяет точку входа для приложения.
//
#include "stdafx.h"

class ISE_Abstract;

#include "..\XrSE_Factory\xrSE_Factory_import_export.h"
#include "resource.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    if (!IsDebuggerPresent())
        Debug._initialize(false);

    Core._initialize("Level", ELogCallback, 1, "fs.ltx", true);
    XrSE_Factory::initialize();
    Tools = xr_new<CLevelTool>();
    LTools = (CLevelTool *)Tools;

    UI = xr_new<CLevelMain>();
    UI->RegisterCommands();
    LUI = (CLevelMain *)UI;

    Scene = xr_new<EScene>();

    UIMainForm *MainForm = xr_new<UIMainForm>();
    HICON icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_LE));

    if (icon)
    {
        SendMessage(EDevice.m_hWnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon));
        SendMessage(EDevice.m_hWnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon));
    }

    ::MainForm = MainForm;
    UI->Push(MainForm, false);
    while (MainForm->Frame())
    {
    }
    xr_delete(MainForm);
    XrSE_Factory::destroy();
    Core._destroy();
    return 0;
}
