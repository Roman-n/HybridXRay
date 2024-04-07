#pragma once
#include "InventoryBox.h"

class CPermanentBox: public CInventoryBox
{
    typedef CInventoryBox inherited;

public:
    CPermanentBox();
    virtual ~CPermanentBox();
};
