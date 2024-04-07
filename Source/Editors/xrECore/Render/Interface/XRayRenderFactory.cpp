#include "pch.h"
#undef RENDER_FACTORY_INTERFACE
#define RENDER_FACTORY_INTERFACE(Class)                       \
    I##Class* XRayRenderFactory::Create##Class()              \
    {                                                         \
        return static_cast<I##Class*>(xr_new<XRay##Class>()); \
    }                                                         \
    void XRayRenderFactory::Destroy##Class(I##Class* pObject) \
    {                                                         \
        xr_delete(static_cast<XRay##Class*>(pObject));        \
    }

#ifndef _EDITOR
RENDER_FACTORY_INTERFACE(UISequenceVideoItem)
RENDER_FACTORY_INTERFACE(UIShader)
RENDER_FACTORY_INTERFACE(StatGraphRender)
RENDER_FACTORY_INTERFACE(ConsoleRender)
RENDER_FACTORY_INTERFACE(RenderDeviceRender)
#ifdef DEBUG
RENDER_FACTORY_INTERFACE(ObjectSpaceRender)
#endif
RENDER_FACTORY_INTERFACE(WallMarkArray)
RENDER_FACTORY_INTERFACE(StatsRender)
#endif   // _EDITOR

RENDER_FACTORY_INTERFACE(EnvironmentRender)
RENDER_FACTORY_INTERFACE(EnvDescriptorMixerRender)
RENDER_FACTORY_INTERFACE(EnvDescriptorRender)
RENDER_FACTORY_INTERFACE(RainRender)
RENDER_FACTORY_INTERFACE(LensFlareRender)
RENDER_FACTORY_INTERFACE(ThunderboltRender)
RENDER_FACTORY_INTERFACE(ThunderboltDescRender)
RENDER_FACTORY_INTERFACE(FlareRender)

RENDER_FACTORY_INTERFACE(FontRender)

RENDER_FACTORY_INTERFACE(ApplicationRender)
