#pragma once
#include "../xrEngine/game_level_cross_table.h"
class CGameLevelCrossTable: public IGameLevelCrossTable
{
public:
    CGameLevelCrossTable(const void* buffer, const u32& buffer_size);
    virtual ~CGameLevelCrossTable();
};
