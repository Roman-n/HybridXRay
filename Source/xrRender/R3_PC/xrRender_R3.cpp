// xrRender_R2.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "../Private/dxRenderFactory.h"
#include "../Private/dxUIRender.h"
#include "../Private/dxDebugRender.h"

extern "C"
{
    void DLL_API InitializeRendering()
    {
        ::Render        = &RImplementation;
        ::RenderFactory = &RenderFactoryImpl;
        ::DU            = &DUImpl;
        //::vid_mode_token			= inited by HW;
        UIRender        = &UIRenderImpl;
#ifdef DEBUG
        DRender = &DebugRenderImpl;
#endif   //	DEBUG
        xrRender_initconsole();
    }
    bool DLL_API SupportsRendering()
    {
        return xrRender_test_hw() ? true : false;
    }
};
