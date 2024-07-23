////////////////////////////////////////////////////////////////////////////
//	Module 		: xrServer_Objects_Abstract.h
//	Created 	: 19.09.2002
//  Modified 	: 18.06.2004
//	Author		: Oles Shyshkovtsov, Alexander Maksimchuk, Victor Reutskiy and Dmitriy Iassenev
//	Description : Server objects
////////////////////////////////////////////////////////////////////////////

SERVER_ENTITY_DECLARE_BEGIN(CSE_Visual, ISE_Visual)
void OnChangeVisual(PropValue* sender);
void OnChangeAnim(PropValue* sender);

public:
CSE_Visual(LPCSTR name = 0);
virtual ~CSE_Visual();

void           visual_read(NET_Packet& P, u16 version);
void           visual_write(NET_Packet& P);

virtual void   set_visual(LPCSTR name, bool load = true);
virtual LPCSTR get_visual() const
{
    return *visual_name;
};
#ifndef XRGAME_EXPORTS
virtual void FillProps(LPCSTR pref, PropItemVec& items);
#endif   // #ifndef XRGAME_EXPORTS

virtual ISE_Visual* visual() = 0;
}
;
add_to_type_list(CSE_Visual)
#define script_type_list save_type_list(CSE_Visual)

    SERVER_ENTITY_DECLARE_BEGIN(CSE_Motion, ISE_Motion) void OnChangeMotion(PropValue* sender);

public:
CSE_Motion(LPCSTR name = 0);
virtual ~CSE_Motion();

void   motion_read(NET_Packet& P);
void   motion_write(NET_Packet& P);

void   set_motion(LPCSTR name);
LPCSTR get_motion() const
{
    return *motion_name;
};

#ifndef XRGAME_EXPORTS
virtual void FillProps(LPCSTR pref, PropItemVec& items);
#endif   // #ifndef XRGAME_EXPORTS

virtual ISE_Motion* motion() = 0;
}
;
add_to_type_list(CSE_Motion)
#define script_type_list save_type_list(CSE_Motion)
