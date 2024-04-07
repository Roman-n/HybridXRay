////////////////////////////////////////////////////////////////////////////
//	Module 		: editor_environment_ambients_sound_id.hpp
//	Created 	: 04.01.2008
//  Modified 	: 04.01.2008
//	Author		: Dmitriy Iassenev
//	Description : editor environment ambients sound identifier class
////////////////////////////////////////////////////////////////////////////

#ifndef EDITOR_WEATHER_AMBIENTS_SOUND_HPP_INCLUDED
#define EDITOR_WEATHER_AMBIENTS_SOUND_HPP_INCLUDED

#ifdef INGAME_EDITOR

#include <boost/noncopyable.hpp>
#include "../xrWeatherEditor/Public/property_holder.hpp"

namespace xrWeatherEditor
{

    class property_holder_collection;

    namespace environment
    {
        namespace sound_channels
        {
            class manager;
        }   // namespace sound_channels

        namespace ambients
        {

            class sound_id: public xrWeatherEditor::property_holder_holder, private boost::noncopyable
            {
            public:
                sound_id(sound_channels::manager const& manager, shared_str const& sound);
                virtual ~sound_id();
                void                     fill(xrWeatherEditor::property_holder_collection* collection);
                inline shared_str const& id() const
                {
                    return m_id;
                }

            private:
                typedef xrWeatherEditor::property_holder property_holder_type;

            public:
                virtual property_holder_type* object();

            private:
                LPCSTR const* xr_stdcall collection();
                u32 xr_stdcall           collection_size();

            private:
                property_holder_type*          m_property_holder;
                sound_channels::manager const& m_manager;
                shared_str                     m_id;
            };   // class sound_id
        }        // namespace ambients
    }            // namespace environment
}   // namespace xrWeatherEditor

#endif   // #ifdef INGAME_EDITOR

#endif   // ifndef EDITOR_WEATHER_AMBIENTS_SOUND_HPP_INCLUDED
