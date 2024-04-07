#pragma once
#include "game_graph_space.h"
class ISE_ALifeLevelChanger
{
public:
    GameGraph::_GRAPH_ID           m_tNextGraphID;
    u32                            m_dwNextNodeID;
    Fvector                        m_tNextPosition;
    Fvector                        m_tAngles;
    shared_str                     m_caLevelToChange;
    shared_str                     m_caLevelPointToChange;
    BOOL                           m_bSilentMode;

    virtual class ISE_ALifeObject* CastALifeObject() = 0;
    virtual class ISE_Abstract*    CastAbstract()    = 0;
};
