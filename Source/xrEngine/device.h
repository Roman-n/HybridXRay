#pragma once

// Note:
// ZNear - always 0.0f
// ZFar  - always 1.0f

// class	ENGINE_API	CResourceManager;
// class	ENGINE_API	CGammaControl;

// #include "shader.h"
// #include "R_Backend.h"

#include "xrDeviceInterface.h"

class engine_impl;

// refs
class ENGINE_API CRenderDevice: public xrDeviceInterface
{
private:
    // Main objects used for creating and rendering the 3D scene
    u32  m_dwWindowStyle;
    RECT m_rcWindowBounds;
    RECT m_rcWindowClient;

    // u32										Timer_MM_Delta;
    // CTimer_paused							Timer;
    // CTimer_paused							TimerGlobal;

    void _Create(LPCSTR shName);
    void _Destroy(BOOL bKeepTextures);
    void _SetupStates();

public:
    //   HWND									m_hWnd;
    LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);

    //	u32										dwFrame;
    //	u32										dwPrecacheFrame;
    u32     dwPrecacheTotal;

    //	u32										dwWidth, dwHeight;
    //	BOOL									b_is_Ready;
    //	BOOL									b_is_Active;
    void    OnWM_Activate(WPARAM wParam, LPARAM lParam);

public:
    // ref_shader								m_WireShader;
    // ref_shader								m_SelectionShader;

public:
    // Registrators
    // CRegistrator	<pureRender			>			seqRender;
    //	CRegistrator	<pureAppActivate	>			seqAppActivate;
    //	CRegistrator	<pureAppDeactivate	>			seqAppDeactivate;
    //	CRegistrator	<pureAppStart		>			seqAppStart;
    //	CRegistrator	<pureAppEnd			>			seqAppEnd;
    // CRegistrator	<pureFrame			>			seqFrame;

    // Engine flow-control
    // float									fTimeDelta;
    // float									fTimeGlobal;
    // u32										dwTimeDelta;
    // u32										dwTimeGlobal;
    // u32										dwTimeContinual;

    // Cameras & projection
    // Fvector									vCameraPosition;
    // Fvector									vCameraDirection;
    // Fvector									vCameraTop;
    // Fvector									vCameraRight;

    // Fmatrix									mView;
    // Fmatrix									mProject;
    // Fmatrix									mFullTransform;

    // float									fFOV;
    // float									fASPECT;

    CRenderDevice():
        dwPrecacheTotal(0)
#ifdef INGAME_EDITOR
        ,
        m_editor_module(0),
        m_editor_initialize(0),
        m_editor_finalize(0),
        m_editor(0),
        m_engine(0)
#endif   // #ifdef INGAME_EDITOR
#ifdef PROFILE_CRITICAL_SECTIONS
            mt_csEnter(MUTEX_PROFILE_ID(CRenderDevice::mt_csEnter)),
        mt_csLeave(MUTEX_PROFILE_ID(CRenderDevice::mt_csLeave)),
#endif   // #ifdef PROFILE_CRITICAL_SECTIONS
    {
        m_hWnd      = NULL;
        b_is_Active = FALSE;
        b_is_Ready  = FALSE;
        Timer.Start();
    };

    virtual void Pause(BOOL bOn, BOOL bTimer, BOOL bSound, LPCSTR reason) override;
    virtual bool Paused() const override;

    // Scene control
    virtual void PreCache(u32 amount, bool b_draw_loadscreen, bool b_wait_user_input) override;
    virtual bool Begin() override;
    virtual void Clear() override;
    virtual void End() override;
    void         FrameMove();

    void         overdrawBegin();
    void         overdrawEnd();

    // Mode control
    void         DumpFlags();

    // Creation & Destroying
    void         ConnectToRender();
    void         Create(void);
    void         Run(void);
    void         Destroy(void);
    void         Reset(bool precache = true);

    void         Initialize(void);
    void         ShutDown(void);

public:

public:
    void xr_stdcall on_idle();
    bool xr_stdcall on_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);

private:
    void                        message_loop();
    virtual void _BCL           AddSeqFrame(pureFrame* f, bool mt);
    virtual void _BCL           RemoveSeqFrame(pureFrame* f);
    virtual CStatsPhysics* _BCL StatPhysics()
    {
        return Statistic;
    }
#ifdef INGAME_EDITOR

public:
    virtual xrWeatherEditor::ide* WeatherEditor() const
    {
        return m_editor;
    }

private:
    void initialize_editor();
    void message_loop_editor();

private:
    typedef xrWeatherEditor::initialize_function_ptr initialize_function_ptr;
    typedef xrWeatherEditor::finalize_function_ptr   finalize_function_ptr;

private:
    HMODULE                 m_editor_module;
    initialize_function_ptr m_editor_initialize;
    finalize_function_ptr   m_editor_finalize;
    xrWeatherEditor::ide*   m_editor;
    engine_impl*            m_engine;
#endif   // #ifdef INGAME_EDITOR
};

extern ENGINE_API bool                    g_bBenchmark;

typedef fastdelegate::FastDelegate0<bool> LOADING_EVENT;
extern ENGINE_API xr_list<LOADING_EVENT> g_loading_events;
class ENGINE_API                         CLoadScreenRenderer: public pureRender
{
public:
    CLoadScreenRenderer();
    void         start(bool b_user_input);
    void         stop();
    virtual void OnRender();

    bool         b_registered;
    bool         b_need_user_input;
};
extern ENGINE_API CLoadScreenRenderer load_screen_renderer;
#ifdef ENGINE_BUILD
extern CRenderDevice* EngineDevice;
#endif
