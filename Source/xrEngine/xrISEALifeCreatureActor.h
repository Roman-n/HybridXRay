#pragma once
class ISE_ALifeCreatureActor
{
public:
    virtual class ISE_ALifeObject* CastALifeObject() = 0;
    virtual class ISE_Abstract*    CastAbstract()    = 0;
};
