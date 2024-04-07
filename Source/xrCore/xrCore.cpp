// xrCore.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#pragma hdrstop

#include <mmsystem.h>
#include <objbase.h>
#include "xrCore.h"
#include "../xrAPI/xrGameManager.h"
#pragma comment(lib, "winmm.lib")

#ifdef DEBUG
#include <malloc.h>
#endif   // DEBUG

XRCORE_API xrCore Core;

#if __has_include(".GitInfo.hpp")
#include ".GitInfo.hpp"
#endif

#define DO_EXPAND(...) __VA_ARGS__##1
#define EXPAND(VAL)    DO_EXPAND(VAL)

#ifdef CI
#if EXPAND(CI) == 1
#undef CI
#endif
#endif

#ifdef GITHUB_ACTIONS
#if EXPAND(GITHUB_ACTIONS) == 1
#undef GITHUB_ACTIONS
#endif
#endif

#ifndef GIT_INFO_CURRENT_COMMIT
#define GIT_INFO_CURRENT_COMMIT unknown
#endif

#ifndef GIT_INFO_CURRENT_BRANCH
#define GIT_INFO_CURRENT_BRANCH unknown
#endif

// computing build id
const pcstr            xrCore::buildDate    = __DATE__;
XRCORE_API const pcstr xrCore::buildCommit  = MACRO_TO_STRING(GIT_INFO_CURRENT_COMMIT);
XRCORE_API const pcstr xrCore::buildBranch  = MACRO_TO_STRING(GIT_INFO_CURRENT_BRANCH);
const u32              xrCore::buildIDLocal = []
{
    u32                Result;

    constexpr int      MonthsCount              = 12;

    static const char* MonthId[MonthsCount]     = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    static const int   DaysInMonth[MonthsCount] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    static const int   startDay                 = 31;
    static const int   startMonth               = 1;
    static const int   startYear                = 2023;

    int                days;
    int                months = 0;
    int                years;

    string16           month;
    string256          buffer;
    xr_strcpy(buffer, __DATE__);
    sscanf(buffer, "%s %d %d", month, &days, &years);

    for (int i = 0; i < MonthsCount; i++)
    {
        if (_stricmp(MonthId[i], month))
            continue;
            months = i;
            break;
        }
    Result = (years - startYear) * 365 + days - startDay;
    for (int i = 0; i < months; i++)
      Result += DaysInMonth[i];

    for (int i = 0; i <= startMonth - 1; i++)
      Result -= DaysInMonth[i];

    return Result;
}();

const xr_string xrCore::buildName = []
{
  xr_string Result;

//#define NEW_RELEASE_VERSION "v.0.8.1b"
#if defined NEW_RELEASE_VERSION
    Result += NEW_RELEASE_VERSION;
#elif defined NIGHT_BUILD_NUMBER
    Result += "GitHub Build: ";
    Result += NIGHT_BUILD_NUMBER;
#else
    Result += "Local Build: ";
    Result += xr_string(buildIDLocal);
#endif

    return Result;
}();


namespace CPU
{
    extern void Detect();
};

static u32                 init_counter = 0;

XRAPI_API extern EGamePath GCurrentGame;
// extern xr_vector<shared_str>* LogFile;

void xrCore::_initialize(LPCSTR _ApplicationName, LogCallback cb, BOOL init_fs, LPCSTR fs_fname, bool editor_fs)
{
    xr_strcpy(ApplicationName, _ApplicationName);

    if (0 == init_counter)
    {
        Editor = editor_fs;
#ifdef XRCORE_STATIC
        _clear87();
        _control87(_PC_53, MCW_PC);
        _control87(_RC_CHOP, MCW_RC);
        _control87(_RC_NEAR, MCW_RC);
        _control87(_MCW_EM, MCW_EM);
#endif
        // Init COM so we can use CoCreateInstance
        // HRESULT co_res =
        if (!strstr(GetCommandLine(), "-editor"))
            CoInitializeEx(NULL, COINIT_MULTITHREADED);

        xr_strcpy(Params, sizeof(Params), GetCommandLine());
        _strlwr_s(Params, sizeof(Params));

        string_path fn, dr, di;

        // application path
        GetModuleFileName(GetModuleHandle(MODULE_NAME), fn, sizeof(fn));
        _splitpath(fn, dr, di, nullptr, nullptr);
        strconcat(sizeof(ApplicationPath), ApplicationPath, dr, di);

#if 0
        // working path
        if( strstr(Params,"-wf") )
        {
            string_path c_name;
            sscanf(strstr(Core.Params,"-wf ")+4,"%[^ ] ",c_name);
            SetCurrentDirectory(c_name);
        }
#endif

        GetCurrentDirectory(sizeof(WorkingPath), WorkingPath);

        // User/Comp Name
        DWORD sz_user = sizeof(UserName);
        GetUserName(UserName, &sz_user);

        DWORD sz_comp = sizeof(CompName);
        GetComputerName(CompName, &sz_comp);

        // Mathematics & PSI detection
        CPU::Detect();

        Memory._initialize(strstr(Params, "-mem_debug") ? TRUE : FALSE);

        DUMP_PHASE;

        InitLog();
        _initialize_cpu();

        // Debug._initialize();

        rtc_initialize();

        if (editor_fs)
            xr_FS = xr_new<ELocatorAPI>();
        else
            xr_FS = xr_new<CLocatorAPI>();

        xr_EFS = xr_new<EFS_Utils>();
        // R_ASSERT (co_res==S_OK);
    }
    if (init_fs)
    {
        u32 flags = 0;
        if (0 != strstr(Params, "-build"))
            flags |= CLocatorAPI::flBuildCopy;
        if (0 != strstr(Params, "-ebuild"))
            flags |= CLocatorAPI::flBuildCopy | CLocatorAPI::flEBuildCopy;
#ifdef DEBUG
        if (strstr(Params, "-cache"))
            flags |= CLocatorAPI::flCacheFiles;
        else
            flags &= ~CLocatorAPI::flCacheFiles;
#endif   // DEBUG
#if 0    // for EDITORS - no cache
		flags 				&=~ CLocatorAPI::flCacheFiles;
#endif   // _EDITOR
        flags |= CLocatorAPI::flScanAppRoot;

#if 1
#ifndef ELocatorAPIH
        if (strstr(Params, "-file_activity") != nullptr)
            flags |= CLocatorAPI::flDumpFileActivity;
#endif
#endif
        FS._initialize(flags, nullptr, fs_fname);

        Msg("'xrCore' %s, %s\n", buildName.c_str(), buildDate);
        Msg("'Branch [%s]', Commit: [%s]\n", buildBranch, buildCommit);
        EFS._initialize();
#ifdef DEBUG
#if 1
        Msg("CRT heap 0x%08x", _get_heap_handle());
        Msg("Process heap 0x%08x", GetProcessHeap());
#endif
#endif   // DEBUG
    }
    SetLogCB(cb);
    init_counter++;
}

#if 1
#include "compression_ppmd_stream.h"
extern compression::ppmd::stream* trained_model;
#endif
void xrCore::_destroy()
{
    --init_counter;
    if (0 == init_counter)
    {
        FS._destroy();
        EFS._destroy();
        xr_delete(xr_FS);
        xr_delete(xr_EFS);

#if 1
        if (trained_model)
        {
            void* buffer = trained_model->buffer();
            xr_free(buffer);
            xr_delete(trained_model);
        }
#endif

        Memory._destroy();
    }
}

#ifndef SHIPPING

//. why ???
#if 0
	BOOL WINAPI DllEntryPoint(HINSTANCE hinstDLL, DWORD ul_reason_for_call, LPVOID lpvReserved)
#else
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD ul_reason_for_call, LPVOID lpvReserved)
#endif
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
        }
        // LogFile.reserve(256);
        break;
        case DLL_THREAD_ATTACH:
            // if (!strstr(GetCommandLine(),"-editor"))
            // CoInitializeEx(NULL, COINIT_MULTITHREADED);
            timeBeginPeriod(1);
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
#ifdef USE_MEMORY_MONITOR
            memory_monitor::flush_each_time(true);
#endif   // USE_MEMORY_MONITOR
            break;
    }
    return TRUE;
}
#endif   // XRCORE_STATIC
