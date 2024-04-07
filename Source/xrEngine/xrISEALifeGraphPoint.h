#pragma once
class ISE_ALifeGraphPoint
{
public:
    u8                          m_tLocations[GameGraph::LOCATION_TYPE_COUNT];
    virtual class ISE_Abstract* CastAbstract() = 0;
};
