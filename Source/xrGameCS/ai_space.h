////////////////////////////////////////////////////////////////////////////
//	Module 		: ai_space.h
//	Created 	: 12.11.2003
//  Modified 	: 12.11.2003
//	Author		: Dmitriy Iassenev
//	Description : AI space class
////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef XRSEFACTORY_EXPORTS
class IGameGraph;
class IGameLevelCrossTable;
class ILevelGraph;
class CGraphEngine;
class CEF_Storage;
class CALifeSimulator;
class CCoverManager;
class CPatrolPathStorage;
class moving_objects;
#endif

class CScriptEngine;

class CAI_Space
{
private:
    friend class CALifeSimulator;
    friend class CALifeGraphRegistry;
    friend class CALifeSpawnRegistry;
    friend class CALifeSpawnRegistry;
    friend class CLevel;

private:
#ifndef XRSEFACTORY_EXPORTS
    IGameGraph*      m_game_graph;
    ILevelGraph*     m_level_graph;
    CGraphEngine*    m_graph_engine;
    CEF_Storage*     m_ef_storage;
    CALifeSimulator* m_alife_simulator;
    CCoverManager*   m_cover_manager;
#endif
    CScriptEngine*   m_script_engine;
#ifndef XRSEFACTORY_EXPORTS
    CPatrolPathStorage* m_patrol_path_storage;
    moving_objects*     m_moving_objects;
#endif
#ifndef XRSEFACTORY_EXPORTS

private:
    void load_from_editor();
    void load(LPCSTR level_name);
    void unload(bool reload = false);
    void patrol_path_storage_raw(IReader& stream);
    void patrol_path_storage(IReader& stream);
    void patrol_path_storage_from_editor();
    void set_alife(CALifeSimulator* alife_simulator);
    void game_graph(IGameGraph* game_graph);
#endif

public:
    CAI_Space();
    virtual ~CAI_Space();
    void                         init();
    IC CScriptEngine&            script_engine() const;
#ifndef XRSEFACTORY_EXPORTS
    IC IGameGraph&               game_graph() const;
    IC IGameGraph*               get_game_graph() const;
    IC ILevelGraph&              level_graph() const;
    IC const ILevelGraph*        get_level_graph() const;
    const IGameLevelCrossTable&  cross_table() const;
    const IGameLevelCrossTable*  get_cross_table() const;
    IC const CPatrolPathStorage& patrol_paths() const;
    IC CEF_Storage&              ef_storage() const;
    IC CGraphEngine&             graph_engine() const;
    IC const CALifeSimulator&    alife() const;
    IC const CALifeSimulator*    get_alife() const;
    IC const CCoverManager&      cover_manager() const;
    IC moving_objects&           moving_objects() const;
#ifdef DEBUG
    void validate(const u32 level_id) const;
#endif
#endif
};

IC CAI_Space&     ai();

extern CAI_Space* g_ai_space;

#include "ai_space_inline.h"
