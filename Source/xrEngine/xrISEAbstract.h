#pragma once
#include "ShapeData.h"
#include "xrISEALifeObject.h"
#include "xrISEALifeGraphPoint.h"
#include "gametype_chooser.h"
class PropItem;
DEFINE_VECTOR(PropItem*, PropItemVec, PropItemIt);
class ISE_Visual
{
public:
    shared_str visual_name;
    shared_str startup_animation;
    enum
    {
        flObstacle = (1 << 0)
    };
    Flags8 flags;

public:
    virtual void   set_visual(LPCSTR name, bool load = true) = 0;
    virtual LPCSTR get_visual() const                        = 0;
};
class ISE_Motion
{
public:
    shared_str motion_name;

public:
    virtual LPCSTR get_motion() const = 0;
};
class CDUInterface;
#pragma pack(push, 1)
struct visual_data
{
    Fmatrix     matrix;
    ISE_Visual* visual;
};   // struct visual_data
#pragma pack(pop)

class ISE_Shape: public CShapeData
{
public:
    virtual void assign_shapes(CShapeData::shape_def* shapes, u32 cnt) = 0;
};

struct ISE_AbstractLEOwner
{
    virtual void get_bone_xform(LPCSTR name, Fmatrix& xform) = 0;
};

class ENGINE_API ISE_Abstract
{
public:
    enum
    {
        flUpdateProperties           = u32(1 << 0),
        flVisualChange               = u32(1 << 1),
        flVisualAnimationChange      = u32(1 << 2),
        flMotionChange               = u32(1 << 3),
        flVisualAnimationPauseChange = u32(1 << 4),
        flSpawnDestroyOnSpawn        = u32(1 << 5),
    };
    Flags32 m_editor_flags;
    IC void set_editor_flag(u32 mask)
    {
        m_editor_flags.set(mask, TRUE);
    }

public:
    BOOL                net_Ready;
    BOOL                net_Processed;   // Internal flag for connectivity-graph

    u16                 m_wVersion;
    u16                 m_script_version;
    u16                 RespawnTime;

    u16                 ID;           // internal ID
    u16                 ID_Parent;    // internal ParentID, 0xffff means no parent
    u16                 ID_Phantom;   // internal PhantomID, 0xffff means no phantom
    class xrClientData* owner;

    // spawn data
    shared_str          s_name;
    //.	u8								s_gameid;
    GameTypeChooser     m_gameType;
    u8                  s_RP;
    Flags16             s_flags;   // state flags
    xr_vector<u16>      children;

    // update data
    Fvector             o_Position;
    Fvector             o_Angle;
    CLASS_ID            m_tClassID;
    int                 m_script_clsid;
    shared_str          m_ini_string;
    CInifile*           m_ini_file;

    // for ALife control
    bool                m_bALifeControl;
    ALife::_SPAWN_ID    m_tSpawnID;

    // ALife spawn params
    // obsolete, just because we hope to uncomment all this stuff
    Flags32             m_spawn_flags;

    // client object custom data serialization
    xr_vector<u8>       client_data;

public:
    virtual void             Spawn_Write(NET_Packet& tNetPacket, BOOL bLocal) = 0;
    virtual BOOL             Spawn_Read(NET_Packet& tNetPacket)               = 0;
    virtual void             FillProp(LPCSTR pref, PropItemVec& items);
    virtual void             on_render(CDUInterface* du, ISE_AbstractLEOwner* owner, bool bSelected, const Fmatrix& parent, int priority, bool strictB2F);
    virtual visual_data*     visual_collection() const;
    virtual u32              visual_collection_size() const;
    virtual void             set_additional_info(void* info);
    virtual LPCSTR           name() const             = 0;
    virtual void             set_name(LPCSTR)         = 0;
    virtual LPCSTR           name_replace() const     = 0;
    virtual void             set_name_replace(LPCSTR) = 0;
    virtual Fvector&         position()               = 0;
    virtual Fvector&         angle()                  = 0;
    virtual Flags16&         flags()                  = 0;
    virtual ISE_Shape*       shape()                  = 0;
    virtual ISE_Visual*      visual()                 = 0;
    virtual ISE_Motion*      motion()                 = 0;
    virtual bool             validate()               = 0;
    virtual ISE_ALifeObject* CastALifeObject()
    {
        return 0;
    }
    virtual ISE_ALifeGraphPoint* CastALifeGraphPoint()
    {
        return 0;
    }

    virtual void UPDATE_Read(NET_Packet& tNetPacket)  = 0;
    virtual void UPDATE_Write(NET_Packet& tNetPacket) = 0;
};
