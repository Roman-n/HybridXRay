#pragma once
class ISE_ALifeSpaceRestrictor
{
public:
    u8                             m_space_restrictor_type;
    virtual class ISE_Shape*       shape()           = 0;
    virtual class ISE_ALifeObject* CastALifeObject() = 0;
    virtual class ISE_Abstract*    CastAbstract()    = 0;
};
