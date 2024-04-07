#pragma once
#include "inventory_space.h"
#include "GameObject.h"
#include "InventoryBox.h"

class CPersonalBox: public CInventoryBox
{
    typedef CInventoryBox inherited;

public:
    CPersonalBox(){};
    virtual ~CPersonalBox(){};

    virtual bool can_take() const
    {
        return true;
    }
};
