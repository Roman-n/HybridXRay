#pragma once

#include "xrSEFactoryManager.h"

IC CSE_Abstract* F_entity_Create(LPCSTR section)
{
    ISE_Abstract* i = g_SEFactoryManager->create_entity(section);
    CSE_Abstract* j = smart_cast<CSE_Abstract*>(i);
    return (j);
}

IC void F_entity_Destroy(CSE_Abstract*& i)
{
    ISE_Abstract* j = i;
    g_SEFactoryManager->destroy_entity(j);
    i = 0;
}
