////////////////////////////////////////////////////////////////////////////
//	Module 		: ide.hpp
//	Created 	: 11.12.2007
//  Modified 	: 29.12.2007
//	Author		: Dmitriy Iassenev
//	Description : editor ide function
////////////////////////////////////////////////////////////////////////////

#ifndef IDE_HPP_INCLUDED
#define IDE_HPP_INCLUDED

#ifdef INGAME_EDITOR

#include "../xrWeatherEditor/Public/ide.hpp"

namespace xrWeatherEditor
{
    class ide;
}   // namespace xrWeatherEditor

inline xrWeatherEditor::ide& ide()
{
    VERIFY(Device->WeatherEditor());
    return (*Device->WeatherEditor());
}

#endif   // #ifdef INGAME_EDITOR

#endif   // ifndef IDE_HPP_INCLUDED
