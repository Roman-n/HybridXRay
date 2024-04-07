#include "pch.hpp"

#pragma managed(push, off)
#include <windows.h>
#include "Public/engine.hpp"
#pragma managed(pop)

#include "window_weather.h"
#include "window_ide.h"
#include "window_view.h"
#include "window_weather_editor.h"

using System::Object;
using xrWeatherEditor::window_weather;

Void window_weather::window_weather_Leave(Object ^ sender, EventArgs ^ e)
{
    m_ide->view().property_grid(PropertyGrid);
}

Void window_weather::SaveButton_Click(Object ^ sender, EventArgs ^ e)
{
    m_ide->engine().save_weathers();
}

Void window_weather::ReloadWeatherButton_Click(Object ^ sender, EventArgs ^ e)
{
    m_ide->engine().reload_current_weather();

#pragma message("Dima to Dima: we can make this more optimal")
    m_ide->weather_editor().fill_weathers();
}

Void window_weather::ReloadAllWeathersButton_Click(Object ^ sender, EventArgs ^ e)
{
    m_ide->engine().reload_weathers();
    m_ide->weather_editor().fill_weathers();
}
