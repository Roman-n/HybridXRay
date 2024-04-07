#ifndef __X_RAY_H__
#define __X_RAY_H__

// refs
class ENGINE_API CGameFont;

#include "FactoryPtr.h"
#include "ApplicationRender.h"

// definition
class ENGINE_API CApplication: public pureFrame, public IEventReceiver
{
    friend class dxApplicationRender;

    // levels
    struct sLevelInfo
    {
        char* folder;
        char* name;
    };

public:
    string2048 ls_header;
    string2048 ls_tip_number;
    string2048 ls_tip;

private:
    FactoryPtr<IApplicationRender> m_pRender;

    int                            max_load_stage;

    int                            load_stage;

    u32                            ll_dwReference;

private:
    EVENT eQuit;
    EVENT eStart;
    EVENT eStartLoad;
    EVENT eDisconnect;
    EVENT eConsole;
    EVENT eStartMPDemo;

    void  Level_Append(LPCSTR lname);

public:
    CGameFont*            pFontSystem;

    // Levels
    xr_vector<sLevelInfo> Levels;
    u32                   Level_Current;
    virtual void          Level_Scan();
    virtual int           Level_ID(LPCSTR name, LPCSTR ver, bool bSet);
    virtual void          Level_Set(u32 ID);
    virtual void          LoadAllArchives();
    virtual CInifile*     GetArchiveHeader(LPCSTR name, LPCSTR ver);

    // Loading
    virtual void          LoadBegin();
    virtual void          LoadEnd();
    virtual void          LoadTitleInt(LPCSTR str1, LPCSTR str2, LPCSTR str3);
    virtual void          LoadStage();
    virtual void          LoadSwitch();
    virtual void          LoadDraw();

    virtual void          OnEvent(EVENT E, u64 P1, u64 P2);

    // Other
    CApplication();
    virtual ~CApplication();

    virtual void _BCL OnFrame();
    virtual void      load_draw_internal();
    virtual void      destroy_loading_shaders();
};

extern ENGINE_API CApplication* pApp;

#endif   //__XR_BASE_H__
