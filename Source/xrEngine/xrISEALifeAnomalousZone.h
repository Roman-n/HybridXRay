#pragma once
class ISE_ALifeAnomalousZone
{
public:
    float                          m_offline_interactive_radius;
    u32                            m_artefact_position_offset;
    u16                            m_artefact_spawn_count;
    virtual class ISE_ALifeObject* CastALifeObject() = 0;
    virtual class ISE_Abstract*    CastAbstract()    = 0;
};
