#pragma once

#include "pure.h"
#include "stats.h"
#include "FactoryPtr.h"
#include "RenderDeviceRender.h"

#ifdef INGAME_EDITOR
#include "../xrWeatherEditor/Public/interfaces.hpp"
#endif   // #ifdef INGAME_EDITOR

#define VIEWPORT_NEAR                     0.2f

#define DEVICE_RESET_PRECACHE_FRAME_COUNT 10

class ENGINE_API xrDeviceInterface
{
public:
    xrDeviceInterface();
    virtual ~xrDeviceInterface();

public:
#ifdef INGAME_EDITOR
    virtual xrWeatherEditor::ide* WeatherEditor() const
    {
        return nullptr;
    }
#endif
#ifndef MASTER_GOLD
    virtual bool IsEditorMode()
    {
        return false;
    }
#else
    constexpr bool IsEditorMode()
    {
        return false;
    }
#endif
    virtual bool Paused() const               = 0;
    virtual void Reset(bool precache = false) = 0;
    IC u32       TimerAsync()
    {
        return TimerGlobal.GetElapsed_ms();
    }
    IC u32 TimerAsync_MMT()
    {
        return TimerMM.GetElapsed_ms() + Timer_MM_Delta;
    }
    IC CTimer* GetTimerGlobal()
    {
        return &TimerGlobal;
    }
    virtual void DumpResourcesMemoryUsage()
    {
        m_pRender->ResourcesDumpMemoryUsage();
    }

    Fmatrix                                  mInvFullTransform;
    CTimer                                   TimerMM;
    CRegistrator<pureFrame>                  seqFrameMT;
    CRegistrator<pureDeviceReset>            seqDeviceReset;
    xr_vector<fastdelegate::FastDelegate0<>> seqParallel;
    CStats*                                  Statistic;
    float                                    fWidth_2, fHeight_2;
    IRenderDeviceRender*                     m_pRender;
    IC void                                  time_factor(const float& time_factor)
    {
        Timer.time_factor(time_factor);
        TimerGlobal.time_factor(time_factor);
    }

    IC const float& time_factor() const
    {
        VERIFY(Timer.time_factor() == TimerGlobal.time_factor());
        return (Timer.time_factor());
    }

    // Multi-threading
    xrCriticalSection mt_csEnter;
    xrCriticalSection mt_csLeave;
    volatile BOOL     mt_bMustExit;

    ICF void          remove_from_seq_parallel(const fastdelegate::FastDelegate0<>& delegate)
    {
        xr_vector<fastdelegate::FastDelegate0<>>::iterator I = std::find(seqParallel.begin(), seqParallel.end(), delegate);
        if (I != seqParallel.end())
            seqParallel.erase(I);
    }
    virtual void Pause(BOOL bOn, BOOL bTimer, BOOL bSound, LPCSTR reason)             = 0;
    virtual void PreCache(u32 amount, bool b_draw_loadscreen, bool b_wait_user_input) = 0;
    virtual void Clear()                                                              = 0;
    BOOL         m_bNearer;
    IC void      SetNearer(BOOL enabled)
    {
        if (enabled && !m_bNearer)
        {
            m_bNearer = TRUE;
            mProject._43 -= EPS_L;
        }
        else if (!enabled && m_bNearer)
        {
            m_bNearer = FALSE;
            mProject._43 += EPS_L;
        }
        m_pRender->SetCacheXform(mView, mProject);
        // R_ASSERT(0);
        //	TODO: re-implement set projection
        // RCache.set_xform_project			(mProject);
    }
    virtual bool Begin() = 0;
    virtual void End()   = 0;

public:
    bool dwMaximized;
    u32  dwWidth;
    u32  dwHeight;
    u32  dwPrecacheFrame;
    BOOL b_is_Ready;
    BOOL b_is_Active;

public:
    // Engine flow-control
    s32     dwFrame;

    float   fTimeDelta;
    float   fTimeGlobal;
    s32     dwTimeDelta;
    u32     dwTimeGlobal;
    u32     dwTimeContinual;

    Fvector vCameraPosition;
    Fvector vCameraDirection;
    Fvector vCameraTop;
    Fvector vCameraRight;

    Fmatrix mView;
    Fmatrix mProject;
    Fmatrix mFullTransform;

    // Copies of corresponding members. Used for synchronization.
    Fvector vCameraPosition_saved;

    Fmatrix mView_saved;
    Fmatrix mProject_saved;
    Fmatrix mFullTransform_saved;

    float   fFOV;
    float   fASPECT;

protected:
    u32           Timer_MM_Delta;
    CTimer_paused Timer;
    CTimer_paused TimerGlobal;

public:
    // Registrators
    CRegistrator<pureRender>                  seqRender;
    CRegistrator<pureAppActivate>             seqAppActivate;
    CRegistrator<pureAppDeactivate>           seqAppDeactivate;
    CRegistrator<pureAppStart>                seqAppStart;
    CRegistrator<pureAppEnd>                  seqAppEnd;
    CRegistrator<pureFrame>                   seqFrame;
    CRegistrator<pureScreenResolutionChanged> seqResolutionChanged;
    CRegistrator<pureDrawUI>                  seqDrawUI;

    HWND                                      m_hWnd;
    //	CStats*									Statistic;

public:
    virtual CStatsPhysics* _BCL StatPhysics()                      = 0;
    virtual void _BCL           AddSeqFrame(pureFrame* f, bool mt) = 0;
    virtual void _BCL           RemoveSeqFrame(pureFrame* f)       = 0;
};
