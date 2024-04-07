#pragma once
#include "xrGameMaterialLibraryInterface.h"
class ENGINE_API CGameMtlLibrary: public xrGameMaterialLibraryInterface
{
    int material_index;
    int material_pair_index;
    BENCH_SEC_SCRAMBLEMEMBER1

    GameMtlVec     materials;
    GameMtlPairVec material_pairs;

    u32            material_count;
    GameMtlPairVec material_pairs_rt;

public:
    CGameMtlLibrary();
    virtual ~CGameMtlLibrary();
    virtual void Unload()
    {
        material_count = 0;
        material_pairs_rt.clear();
        for (GameMtlIt m_it = materials.begin(); materials.end() != m_it; ++m_it)
            xr_delete(*m_it);
        materials.clear();
        for (GameMtlPairIt p_it = material_pairs.begin(); material_pairs.end() != p_it; ++p_it)
            xr_delete(*p_it);
        material_pairs.clear();
    }
    // material routine
    virtual GameMtlIt GetMaterialIt(LPCSTR name)
    {
        for (GameMtlIt it = materials.begin(); materials.end() != it; ++it)
            if (0 == strcmpi(*(*it)->m_Name, name))
                return it;
        return materials.end();
    }
    virtual GameMtlIt GetMaterialIt(shared_str& name)
    {
        for (GameMtlIt it = materials.begin(); materials.end() != it; ++it)
            if (name.equal((*it)->m_Name))
                return it;
        return materials.end();
    }
    virtual GameMtlIt GetMaterialItByID(int id)
    {
        for (GameMtlIt it = materials.begin(); materials.end() != it; ++it)
            if ((*it)->ID == id)
                return it;
        return materials.end();
    }
    virtual u32 GetMaterialID(LPCSTR name)
    {
        GameMtlIt it = GetMaterialIt(name);
        return (it == materials.end()) ? GAMEMTL_NONE_ID : (*it)->ID;
    }
    // game
    virtual SGameMtl* GetMaterialByID(s32 id)
    {
        return GetMaterialByIdx(GetMaterialIdx(id));
    }
    virtual u16 GetMaterialIdx(int ID)
    {
        GameMtlIt it = GetMaterialItByID(ID);
        VERIFY(materials.end() != it);
        return (u16)(it - materials.begin());
    }
    virtual u16 GetMaterialIdx(LPCSTR name)
    {
        GameMtlIt it = GetMaterialIt(name);
        VERIFY(materials.end() != it);
        return (u16)(it - materials.begin());
    }
    virtual SGameMtl* GetMaterialByIdx(u16 idx)
    {
        VERIFY(idx < (u16)materials.size());
        return materials[idx];
    }

    virtual GameMtlIt FirstMaterial()
    {
        return materials.begin();
    }
    virtual GameMtlIt LastMaterial()
    {
        return materials.end();
    }
    virtual u32 CountMaterial()
    {
        return materials.size();
    }

    // game
    virtual SGameMtlPair* GetMaterialPair(u16 idx0, u16 idx1)
    {
        R_ASSERT((idx0 < material_count) && (idx1 < material_count));
        return material_pairs_rt[idx1 * material_count + idx0];
    }

    virtual GameMtlPairIt FirstMaterialPair()
    {
        return material_pairs.begin();
    }
    virtual GameMtlPairIt LastMaterialPair()
    {
        return material_pairs.end();
    }

    // IO routines
    virtual void Load();
    virtual bool Save()
    {
        R_ASSERT(false);
        return false;
    }
};

#define GET_RANDOM(a_vector) (a_vector[Random.randI(a_vector.size())])

#define CLONE_MTL_SOUND(_res_, _mtl_pair_, _a_vector_)                               \
    {                                                                                \
        VERIFY2(!_mtl_pair_##->_a_vector_.empty(), _mtl_pair_->dbg_Name());          \
        _res_.clone(GET_RANDOM(_mtl_pair_##->_a_vector_), st_Effect, sg_SourceType); \
    }
#include "../xrapi/xrapi.h"
