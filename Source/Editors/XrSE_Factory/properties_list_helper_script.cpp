////////////////////////////////////////////////////////////////////////////
//	Module 		: properties_list_helper_script.cpp
//	Created 	: 14.07.2004
//  Modified 	: 14.07.2004
//	Author		: Dmitriy Iassenev
//	Description : Properties list helper script export
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "script_space.h"
#include "script_properties_list_helper.h"
#include "ai_space.h"
#include "script_engine.h"
#include "script_token_list.h"
#include "luabind/out_value_policy.hpp"
using namespace luabind;

struct CChooseType
{
};

typedef IPropHelper&(__stdcall* TPHelper)();

CScriptPropertiesListHelper* g_property_list_helper = 0;

void                         load_prop_helper()
{
    g_property_list_helper = xr_new<CScriptPropertiesListHelper>();
}

CScriptPropertiesListHelper* properties_helper()
{
    if (!g_property_list_helper)
    {
        static bool first_time = true;
        if (first_time)
        {
            first_time = false;
            load_prop_helper();
        }
    }
    // ai().script_engine().script_log	(ScriptStorage::eLuaMessageTypeError,"Editor is not started, therefore
    // prop_helper cannot be accessed!");

    return (g_property_list_helper);
}

void CScriptPropertiesListHelper::script_register(lua_State* L)
{
    module(L)
        [class_<PropValue>("prop_value"), class_<PropItemVec>("prop_item_vec"), class_<CaptionValue>("caption_value"),
            class_<CanvasValue>("canvas_value"), class_<ButtonValue>("button_value"), class_<ChooseValue>("choose_value"),
            class_<S8Value>("s8_value"), class_<S16Value>("s16_value"), class_<S32Value>("s32_value"),
            class_<U8Value>("u8_value"), class_<U16Value>("u16_value"), class_<U32Value>("u32_value"),
            class_<FloatValue>("u32_value"), class_<BOOLValue>("bool_value"), class_<VectorValue>("vector_value"),
            class_<ColorValue>("color_value"), class_<RTextValue>("text_value"), class_<Flag8Value>("flag8_value"),
            class_<Flag16Value>("flag16_value"), class_<Flag32Value>("flag32_value"), class_<Token8Value>("token8_value"),
            class_<Token16Value>("token16_value"), class_<Token32Value>("token32_value"),
            //		class_<RToken8Value>("rtoken8_value"),
            //		class_<RToken16Value>("rtoken16_value"),
            //		class_<RToken32Value>("rtoken32_value"),
            class_<RListValue>("list_value"),

            class_<CChooseType>("choose_type")
                .enum_("choose_type")
                    [value("custom", static_cast<int>(smCustom)), value("sound_source", static_cast<int>(smSoundSource)),
                        value("sound_environment", static_cast<int>(smSoundEnv)), value("library_object", static_cast<int>(smObject)),
                        value("engine_shader", static_cast<int>(smEShader)), value("compiler_shader", static_cast<int>(smCShader)),
                        value("particle_effect", static_cast<int>(smPE)), value("particle_system", static_cast<int>(smParticles)),
                        value("texture", static_cast<int>(smTexture)), value("entity", static_cast<int>(smEntityType)),
                        value("spawn_item", static_cast<int>(smSpawnItem)), value("light_animation", static_cast<int>(smLAnim)),
                        value("visual", static_cast<int>(smVisual)), value("skeleton_animations", static_cast<int>(smSkeletonAnims)),
                        value("skeleton_bones", static_cast<int>(smSkeletonBones)), value("material", static_cast<int>(smGameMaterial)),
                        value("game_animation", static_cast<int>(smGameAnim)), value("game_motion", static_cast<int>(smGameSMotions))],

            class_<CScriptPropertiesListHelper>("properties_list_helper")
                .def("vector_on_after_edit", &CScriptPropertiesListHelper::FvectorRDOnAfterEdit)
                .def("vector_on_before_edit", &CScriptPropertiesListHelper::FvectorRDOnBeforeEdit)
                //			.def("vector_on_draw",			&CScriptPropertiesListHelper::FvectorRDOnDraw)
                .def("float_on_after_edit", &CScriptPropertiesListHelper::floatRDOnAfterEdit, policy::out_value<3>())
                .def("float_on_before_edit", &CScriptPropertiesListHelper::floatRDOnBeforeEdit, policy::out_value<3>())
                //			.def("float_on_draw",			&CScriptPropertiesListHelper::floatRDOnDraw)
                .def("name_after_edit", +[](CScriptPropertiesListHelper* props, PropValue* sender, LPCSTR* edit_val)
                {
                        return props->NameAfterEdit(sender, *edit_val);
                }, policy::out_value<3>())
                .def("name_before_edit", +[](CScriptPropertiesListHelper* props, PropValue* sender, LPCSTR* edit_val)
                {
                    return props->NameBeforeEdit(sender, *edit_val);
                }, policy::out_value<3>())
                //			.def("name_on_draw",			&CScriptPropertiesListHelper::NameDraw)

                .def("create_caption", &CScriptPropertiesListHelper::CreateCaption)
                .def("create_canvas", &CScriptPropertiesListHelper::CreateCanvas)
                .def("create_button", &CScriptPropertiesListHelper::CreateButton)

                .def(
                    "create_choose",
                    static_cast<ChooseValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u32)>(&CScriptPropertiesListHelper::CreateChoose))
                .def(
                    "create_choose",
                    static_cast<ChooseValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u32, LPCSTR)>(&CScriptPropertiesListHelper::CreateChoose))
                .def(
                    "create_choose",
                    static_cast<ChooseValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u32, LPCSTR, LPCSTR)>
                        (&CScriptPropertiesListHelper::CreateChoose))
                .def(
                    "create_choose",
                    static_cast<ChooseValue *(CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u32, LPCSTR, LPCSTR, u32)>
                        (&CScriptPropertiesListHelper::CreateChoose))

                //			.def("create_s8", (S8Value *(CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR,  luabind::object ,
                //LPCSTR ))					(&CScriptPropertiesListHelper::CreateS8)) 			.def("create_s8", (S8Value
                //*(CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR,  luabind::object , LPCSTR ,  s8))
                //(&CScriptPropertiesListHelper::CreateS8)) 			.def("create_s8", (S8Value
                //*(CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR,  luabind::object , LPCSTR ,  s8,  s8))
                //(&CScriptPropertiesListHelper::CreateS8)) 			.def("create_s8", (S8Value
                //*(CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR,  luabind::object , LPCSTR ,  s8,  s8,  s8))
                //(&CScriptPropertiesListHelper::CreateS8))

                .def(
                    "create_s16",
                    static_cast<S16Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR)>(&CScriptPropertiesListHelper::CreateS16))
                .def(
                    "create_s16",
                    static_cast<S16Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, s16)>(&CScriptPropertiesListHelper::CreateS16))
                .def(
                    "create_s16",
                    static_cast<S16Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, s16, s16)>(&CScriptPropertiesListHelper::CreateS16))
                .def(
                    "create_s16",
                    static_cast<S16Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, s16, s16, s16)>
                        (&CScriptPropertiesListHelper::CreateS16))

                .def(
                    "create_s32",
                    static_cast<S32Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR)>(&CScriptPropertiesListHelper::CreateS32))
                .def(
                    "create_s32",
                    static_cast<S32Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, s32)>(&CScriptPropertiesListHelper::CreateS32))
                .def(
                    "create_s32",
                    static_cast<S32Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, s32, s32)>(&CScriptPropertiesListHelper::CreateS32))
                .def(
                    "create_s32",
                    static_cast<S32Value *
                        (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, s32, s32, s32)>(&CScriptPropertiesListHelper::CreateS32))

                .def(
                    "create_u8",
                    static_cast<U8Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR)>(&CScriptPropertiesListHelper::CreateU8))
                .def(
                    "create_u8",
                    static_cast<U8Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u8)>(&CScriptPropertiesListHelper::CreateU8))
                .def(
                    "create_u8",
                    static_cast<U8Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u8, u8)>(&CScriptPropertiesListHelper::CreateU8))
                .def(
                    "create_u8",
                    static_cast<U8Value *
                        (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u8, u8, u8)>(&CScriptPropertiesListHelper::CreateU8))

                .def(
                    "create_u16",
                    static_cast<U16Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR)>(&CScriptPropertiesListHelper::CreateU16))
                .def(
                    "create_u16",
                    static_cast<U16Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u16)>(&CScriptPropertiesListHelper::CreateU16))
                .def(
                    "create_u16",
                    static_cast<U16Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u16, u16)>(&CScriptPropertiesListHelper::CreateU16))
                .def(
                    "create_u16",
                    static_cast<U16Value *
                        (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u16, u16, u16)>(&CScriptPropertiesListHelper::CreateU16))

                .def(
                    "create_u32",
                    static_cast<U32Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR)>(&CScriptPropertiesListHelper::CreateU32))
                .def(
                    "create_u32",
                    static_cast<U32Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u32)>(&CScriptPropertiesListHelper::CreateU32))
                .def(
                    "create_u32",
                    static_cast<U32Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u32, u32)>(&CScriptPropertiesListHelper::CreateU32))
                .def(
                    "create_u32",
                    static_cast<U32Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u32, u32, u32)>
                        (&CScriptPropertiesListHelper::CreateU32))
                .def(
                    "create_float",
                    static_cast<FloatValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR)>(&CScriptPropertiesListHelper::CreateFloat))
                .def(
                    "create_float",
                    static_cast<FloatValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, float)>(&CScriptPropertiesListHelper::CreateFloat))
                .def(
                    "create_float",
                    static_cast<FloatValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, float, float)>
                        (&CScriptPropertiesListHelper::CreateFloat))
                .def(
                    "create_float",
                    static_cast<FloatValue *
                        (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, float, float, float)>(&CScriptPropertiesListHelper::CreateFloat))
                .def(
                    "create_float",
                    static_cast<FloatValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, float, float, float, int)>
                        (&CScriptPropertiesListHelper::CreateFloat))

                .def("create_bool", &CScriptPropertiesListHelper::CreateBOOL)

                .def(
                    "create_vector",
                    static_cast<VectorValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR)>(&CScriptPropertiesListHelper::CreateVector))
                .def(
                    "create_vector",
                    static_cast<VectorValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, float)>(&CScriptPropertiesListHelper::CreateVector))
                .def(
                    "create_vector",
                    static_cast<VectorValue *
                        (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, float, float)>(&CScriptPropertiesListHelper::CreateVector))
                .def(
                    "create_vector",
                    static_cast<VectorValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, float, float, float)>
                        (&CScriptPropertiesListHelper::CreateVector))
                .def(
                    "create_vector",
                    static_cast<VectorValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, float, float, float, int)>
                        (&CScriptPropertiesListHelper::CreateVector))
                .def(
                    "create_flag8",
                    static_cast<Flag8Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u8)>(&CScriptPropertiesListHelper::CreateFlag8))
                .def(
                    "create_flag8",
                    static_cast<Flag8Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u8, LPCSTR)>
                        (&CScriptPropertiesListHelper::CreateFlag8))
                .def(
                    "create_flag8",
                    static_cast<Flag8Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u8, LPCSTR, LPCSTR)>
                        (&CScriptPropertiesListHelper::CreateFlag8))
                .def(
                    "create_flag8",
                    static_cast<Flag8Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u8, LPCSTR, LPCSTR, u32)>
                        (&CScriptPropertiesListHelper::CreateFlag8))

                .def(
                    "create_flag16",
                    static_cast<Flag16Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u16)>(&CScriptPropertiesListHelper::CreateFlag16))
                .def(
                    "create_flag16",
                    static_cast<Flag16Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u16, LPCSTR)>
                        (&CScriptPropertiesListHelper::CreateFlag16))
                .def(
                    "create_flag16",
                    static_cast<Flag16Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u16, LPCSTR, LPCSTR)>
                        (&CScriptPropertiesListHelper::CreateFlag16))
                .def(
                    "create_flag16",
                    static_cast<Flag16Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u16, LPCSTR, LPCSTR, u32)>
                        (&CScriptPropertiesListHelper::CreateFlag16))

                .def(
                    "create_flag32",
                    static_cast<Flag32Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u32)>(&CScriptPropertiesListHelper::CreateFlag32))
                .def(
                    "create_flag32",
                    static_cast<Flag32Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u32, LPCSTR)>
                        (&CScriptPropertiesListHelper::CreateFlag32))
                .def(
                    "create_flag32",
                    static_cast<Flag32Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u32, LPCSTR, LPCSTR)>
                        (&CScriptPropertiesListHelper::CreateFlag32))
                .def(
                    "create_flag32",
                    static_cast<Flag32Value * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, u32, LPCSTR, LPCSTR, u32)>
                        (&CScriptPropertiesListHelper::CreateFlag32))

                .def("create_token8", &CScriptPropertiesListHelper::CreateToken8)
                .def("create_token16", &CScriptPropertiesListHelper::CreateToken16)
                .def("create_token32", &CScriptPropertiesListHelper::CreateToken32)

                //			.def("create_rtoken8",	&CScriptPropertiesListHelper::CreateRToken8)
                //			.def("create_rtoken16",	&CScriptPropertiesListHelper::CreateRToken16)
                //			.def("create_rtoken32",	&CScriptPropertiesListHelper::CreateRToken32)

                .def("create_list", &CScriptPropertiesListHelper::CreateRList)

                .def("create_color", &CScriptPropertiesListHelper::CreateColor)
                .def("create_fcolor", &CScriptPropertiesListHelper::CreateFColor)
                .def("create_vcolor", &CScriptPropertiesListHelper::CreateVColor)

                .def("create_text", &CScriptPropertiesListHelper::CreateRText)

                    
                .def(
                    "create_time",
                    static_cast<FloatValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR)>(&CScriptPropertiesListHelper::CreateTime))
                .def(
                    "create_time",
                    static_cast<FloatValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, float)>(&CScriptPropertiesListHelper::CreateTime))
                .def(
                    "create_time",
                    static_cast<FloatValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, float, float)>
                        (&CScriptPropertiesListHelper::CreateTime))

                .def(
                    "create_angle",
                    static_cast<FloatValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR)>(&CScriptPropertiesListHelper::CreateAngle))
                .def(
                    "create_angle",
                    static_cast<FloatValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, float)>(&CScriptPropertiesListHelper::CreateAngle))
                .def(
                    "create_angle",
                    static_cast<FloatValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, float, float)>
                        (&CScriptPropertiesListHelper::CreateAngle))
                .def(
                    "create_angle",
                    static_cast<FloatValue *
                        (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, float, float, float)>(&CScriptPropertiesListHelper::CreateAngle))
                .def(
                    "create_angle",
                    static_cast<FloatValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, float, float, float, int)>
                        (&CScriptPropertiesListHelper::CreateAngle))

                .def(
                    "create_vangle",
                    static_cast<VectorValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR)>(&CScriptPropertiesListHelper::CreateAngle3))
                .def(
                    "create_vangle",
                    static_cast<VectorValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, float)>(&CScriptPropertiesListHelper::CreateAngle3))
                .def(
                    "create_vangle",
                    static_cast<VectorValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, float, float)>
                        (&CScriptPropertiesListHelper::CreateAngle3))
                .def(
                    "create_vangle",
                    static_cast<VectorValue *
                        (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, float, float, float)>(&CScriptPropertiesListHelper::CreateAngle3))
                .def(
                    "create_vangle",
                    static_cast<VectorValue * (CScriptPropertiesListHelper::*)(PropItemVec*, LPCSTR, luabind::object, LPCSTR, float, float, float, int)>
                        (&CScriptPropertiesListHelper::CreateAngle3)),

            def("properties_helper", &properties_helper)];
}
