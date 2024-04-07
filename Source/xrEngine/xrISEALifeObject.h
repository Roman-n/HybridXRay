#pragma once
#include "xrISEALifeCreatureActor.h"
#include "xrISEALifeDynamicObject.h"
#include "xrISEALifeLevelChanger.h"
#include "xrISEALifeSpaceRestrictor.h"
#include "xrISEALifeAnomalousZone.h"
#include "alife_space.h"
#include "game_graph_space.h"
class ISE_ALifeObject
{
public:
    enum
    {
        flUseSwitches       = u32(1) << 0,
        flSwitchOnline      = u32(1) << 1,
        flSwitchOffline     = u32(1) << 2,
        flInteractive       = u32(1) << 3,
        flVisibleForAI      = u32(1) << 4,
        flUsefulForAI       = u32(1) << 5,
        flOfflineNoMove     = u32(1) << 6,
        flUsedAI_Locations  = u32(1) << 7,
        flGroupBehaviour    = u32(1) << 8,
        flCanSave           = u32(1) << 9,
        flVisibleForMap     = u32(1) << 10,
        flUseSmartTerrains  = u32(1) << 11,
        flCheckForSeparator = u32(1) << 12,
    };
    GameGraph::_GRAPH_ID            m_tGraphID;
    float                           m_fDistance;
    bool                            m_bOnline;
    bool                            m_bDirectControl;
    u32                             m_tNodeID;
    flags32                         m_flags;
    ALife::_STORY_ID                m_story_id;
    ALife::_SPAWN_STORY_ID          m_spawn_story_id;
    virtual ISE_ALifeDynamicObject* CastALifeDynamicObject()
    {
        return 0;
    }
    virtual ISE_ALifeCreatureActor* CastALifeCreatureActor()
    {
        return 0;
    }
    virtual ISE_ALifeLevelChanger* CastALifeLevelChanger()
    {
        return 0;
    }
    virtual ISE_ALifeSpaceRestrictor* CastALifeSpaceRestricto()
    {
        return 0;
    }
    virtual ISE_ALifeAnomalousZone* CastALifeAnomalousZone()
    {
        return 0;
    }
    virtual class ISE_Abstract* CastAbstract()            = 0;
    virtual bool                used_ai_locations() const = 0;
};
