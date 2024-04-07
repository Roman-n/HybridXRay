////////////////////////////////////////////////////////////////////////////
//	Module 		: ide_impl.hpp
//	Created 	: 04.12.2007
//  Modified 	: 04.12.2007
//	Author		: Dmitriy Iassenev
//	Description : IDE implementation class
////////////////////////////////////////////////////////////////////////////

#ifndef IDE_IMPL_HPP_INCLUDED
#define IDE_IMPL_HPP_INCLUDED

#pragma managed(push, off)
#include <windows.h>
#include "../xrcore/fastdelegate.h"
#include <utility>
#include "Public/ide.hpp"
#pragma managed(pop)

#include <vcclr.h>

namespace xrWeatherEditor
{
    ref class window_ide;
    class engine;
    class property_holder_holder;
}   // namespace xrWeatherEditor

class ide_impl: public xrWeatherEditor::ide
{
public:
    typedef xrWeatherEditor::window_ide      window_ide;
    typedef xrWeatherEditor::property_holder property_holder;

public:
    ide_impl(xrWeatherEditor::engine* engine);
    virtual ~ide_impl();
    void window(window_ide ^ window);
    window_ide ^ window();
    void on_idle_start();
    void on_idle();
    void on_idle_end();
    bool idle() const;

public:
    virtual HWND main_handle();
    virtual HWND view_handle();
    virtual void run();
    virtual void on_load_finished();
    virtual void pause();

public:
    virtual property_holder* create_property_holder(LPCSTR display_name, xrWeatherEditor::property_holder_collection* collection, xrWeatherEditor::property_holder_holder* holder);
    virtual void             destroy(property_holder*& property_holder);
    virtual void             environment_levels(property_holder* property_holder);
    virtual void             environment_weathers(property_holder* property_holder);
    virtual void             weather_editor_setup(weathers_getter_type const& weathers_getter, weathers_size_getter_type const& weathers_size_getter, frames_getter_type const& frames_getter, frames_size_getter_type const& frames_size_getter);

private:
    xrWeatherEditor::engine* m_engine;
    gcroot<window_ide ^>     m_window;
    bool                     m_paused;
    bool                     m_in_idle;
};   // class ide

#endif   // ifndef IDE_IMPL_HPP_INCLUDED
