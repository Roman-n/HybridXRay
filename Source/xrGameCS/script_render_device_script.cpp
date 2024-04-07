////////////////////////////////////////////////////////////////////////////
//	Module 		: script_render_device_script.cpp
//	Created 	: 28.06.2004
//  Modified 	: 28.06.2004
//	Author		: Dmitriy Iassenev
//	Description : Script render device script export
////////////////////////////////////////////////////////////////////////////

#include "pch_script.h"
#include "script_render_Device.h"

using namespace luabind;

bool is_device_paused(xrDeviceInterface* d)
{
    return !!Device->Paused();
}

void set_device_paused(xrDeviceInterface* d, bool b)
{
    Device->Pause(b, TRUE, FALSE, "set_device_paused_script");
}

extern ENGINE_API BOOL g_appLoaded;
bool                   is_app_ready()
{
    return !!g_appLoaded;
}

u32 time_global(const xrDeviceInterface* self)
{
    THROW(self);
    return (self->dwTimeGlobal);
}

#pragma optimize("s", on)
void CScriptRenderDevice::script_register(lua_State* L)
{
    module(L)[class_<xrDeviceInterface>("render_device")
                  .def_readonly("width", &xrDeviceInterface::dwWidth)
                  .def_readonly("height", &xrDeviceInterface::dwHeight)
                  .def_readonly("time_delta", &xrDeviceInterface::dwTimeDelta)
                  .def_readonly("f_time_delta", &xrDeviceInterface::fTimeDelta)
                  .def_readonly("cam_pos", &xrDeviceInterface::vCameraPosition)
                  .def_readonly("cam_dir", &xrDeviceInterface::vCameraDirection)
                  .def_readonly("cam_top", &xrDeviceInterface::vCameraTop)
                  .def_readonly("cam_right", &xrDeviceInterface::vCameraRight)
                  //			.def_readonly("view",					&xrDeviceInterface::mView)
                  //			.def_readonly("projection",				&xrDeviceInterface::mProject)
                  //			.def_readonly("full_transform",			&xrDeviceInterface::mFullTransform)
                  .def_readonly("fov", &xrDeviceInterface::fFOV)
                  .def_readonly("aspect_ratio", &xrDeviceInterface::fASPECT)
                  .def("time_global", &time_global)
                  .def_readonly("precache_frame", &xrDeviceInterface::dwPrecacheFrame)
                  .def_readonly("frame", &xrDeviceInterface::dwFrame)
                  .def("is_paused", &is_device_paused)
                  .def("pause", &set_device_paused),
        def("app_ready", &is_app_ready)];
}
