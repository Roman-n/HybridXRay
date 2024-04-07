////////////////////////////////////////////////////////////////////////////
//	Module 		: editor_environment_suns_flare.hpp
//	Created 	: 13.12.2007
//  Modified 	: 04.01.2008
//	Author		: Dmitriy Iassenev
//	Description : editor environment suns flare class
////////////////////////////////////////////////////////////////////////////

#ifndef EDITOR_WEATHER_FLARE_HPP_INCLUDED
#define EDITOR_WEATHER_FLARE_HPP_INCLUDED

#ifdef INGAME_EDITOR

#include <boost/noncopyable.hpp>
#include "../xrWeatherEditor/Public/property_holder.hpp"

namespace xrWeatherEditor
{
    namespace environment
    {
        namespace suns
        {

            class flare: public xrWeatherEditor::property_holder_holder, private boost::noncopyable
            {
            public:
                flare();
                virtual ~flare();
                void fill(xrWeatherEditor::property_holder_collection* collection);

            public:
                typedef xrWeatherEditor::property_holder property_holder;

            public:
                virtual property_holder* object();

            private:
                property_holder* m_property_holder;

            public:
                shared_str m_texture;
                float      m_opacity;
                float      m_position;
                float      m_radius;
            };   // class flare

        }   // namespace suns
    }       // namespace environment
}   // namespace xrWeatherEditor

#endif   // #ifdef INGAME_EDITOR

#endif   // ifndef EDITOR_WEATHER_FLARE_HPP_INCLUDED
