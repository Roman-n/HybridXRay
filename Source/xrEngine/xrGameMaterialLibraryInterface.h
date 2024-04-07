#pragma once

#define GAMEMTL_CURRENT_VERSION    0x0001
//----------------------------------------------------

#define GAMEMTLS_CHUNK_VERSION     0x1000
#define GAMEMTLS_CHUNK_AUTOINC     0x1001
#define GAMEMTLS_CHUNK_MTLS        0x1002
#define GAMEMTLS_CHUNK_MTLS_PAIR   0x1003
//----------------------------------------------------
#define GAMEMTL_CHUNK_MAIN         0x1000
#define GAMEMTL_CHUNK_FLAGS        0x1001
#define GAMEMTL_CHUNK_PHYSICS      0x1002
#define GAMEMTL_CHUNK_FACTORS      0x1003
#define GAMEMTL_CHUNK_FLOTATION    0x1004
#define GAMEMTL_CHUNK_DESC         0x1005
#define GAMEMTL_CHUNK_INJURIOUS    0x1006

#define GAMEMTL_CHUNK_DENSITY      0x1007
#define GAMEMTL_CHUNK_FACTORS_MP   0x1008
//----------------------------------------------------
#define GAMEMTLPAIR_CHUNK_PAIR     0x1000
// #define GAMEMTLPAIR_CHUNK_FLOTATION   0x1001 - obsolete
#define GAMEMTLPAIR_CHUNK_BREAKING 0x1002
#define GAMEMTLPAIR_CHUNK_STEP     0x1003
// #define GAMEMTLPAIR_CHUNK_COLLIDE   	0x1004 - obsolete / rename HIT
#define GAMEMTLPAIR_CHUNK_COLLIDE  0x1005
//----------------------------------------------------

#define GAMEMTL_SUBITEM_COUNT      4

#define GAMEMTL_NONE_ID            u32(-1)
#define GAMEMTL_NONE_IDX           u16(-1)
#define GAMEMTL_FILENAME           "gamemtl.xr"

class xrGameMaterialLibraryInterface;
class ENGINE_API SGameMtl
{
    friend class xrGameMaterialLibraryEditors;
    friend class CGameMtlLibrary;

protected:
    int ID;   // auto number

public:
    enum
    {
        flBreakable         = (1ul << 0ul),
        //		flShootable 	= (1ul<<1ul),
        flBounceable        = (1ul << 2ul),
        flSkidmark          = (1ul << 3ul),
        flBloodmark         = (1ul << 4ul),
        flClimable          = (1ul << 5ul),
        //		flWalkOn		= (1ul<<6ul), // obsolette
        flPassable          = (1ul << 7ul),
        flDynamic           = (1ul << 8ul),
        flLiquid            = (1ul << 9ul),
        flSuppressShadows   = (1ul << 10ul),
        flSuppressWallmarks = (1ul << 11ul),
        flActorObstacle     = (1ul << 12ul),
        flNoRicoshet        = (1ul << 13ul),

        flInjurious         = (1ul << 28ul),   // flInjurious = fInjuriousSpeed > 0.f
        flShootable         = (1ul << 29ul),
        flTransparent       = (1ul << 30ul),
        flSlowDown          = (1ul << 31ul)   // flSlowDown = (fFlotationFactor<1.f)
    };

public:
    shared_str m_Name;
    shared_str m_Desc;

    Flags32    Flags;
    // physics part
    float      fPHFriction;              // ?
    float      fPHDamping;               // ?
    float      fPHSpring;                // ?
    float      fPHBounceStartVelocity;   // ?
    float      fPHBouncing;              // ?
    // shoot&bounce&visibility&flotation
    float      fFlotationFactor;         // 0.f - 1.f   	(1.f-��������� ����������)
    float      fShootFactor;             // 0.f - 1.f	(1.f-��������� ���������������)
    float      fShootFactorMP;           // 0.f - 1.f	(1.f-��������� ���������������)
    float      fBounceDamageFactor;      // 0.f - 100.f
    float      fInjuriousSpeed;          // 0.f - ...	(0.f-�� �������� �������� (�������� ���������� ��������))
    float      fVisTransparencyFactor;   // 0.f - 1.f	(1.f-��������� ����������)
    float      fSndOcclusionFactor;      // 0.f - 1.f    (1.f-��������� ������)
    float      fDensityFactor;

public:
    SGameMtl();
    virtual ~SGameMtl();
    virtual void Load(IReader& fs);
    virtual void Save(IWriter& fs);
    IC int       GetID()
    {
        return ID;
    }
};

DEFINE_VECTOR(SGameMtl*, GameMtlVec, GameMtlIt);
DEFINE_VECTOR(ref_sound, SoundVec, SoundIt);
DEFINE_VECTOR(shared_str, PSVec, PSIt);
#include "WallMarkArray.h"
#include "RenderFactory.h"

class ENGINE_API SGameMtlPair
{
    friend class xrGameMaterialLibraryEditors;
    friend class CGameMtlLibrary;
    xrGameMaterialLibraryInterface* m_Owner;

protected:
    int mtl0;
    int mtl1;

protected:
    int ID;   // auto number
    int ID_parent;

public:
    enum
    {
        //		flFlotation		= (1<<0),
        flBreakingSounds   = (1 << 1),
        flStepSounds       = (1 << 2),
        //		flCollideSounds	= (1<<3),
        flCollideSounds    = (1 << 4),
        flCollideParticles = (1 << 5),
        flCollideMarks     = (1 << 6)
    };
    Flags32                    OwnProps;
    //	properties
    SoundVec                   BreakingSounds;
    SoundVec                   StepSounds;
    SoundVec                   CollideSounds;
    PSVec                      CollideParticles;
    FactoryPtr<IWallMarkArray> m_pCollideMarks;

public:
    SGameMtlPair(xrGameMaterialLibraryInterface* owner);
    virtual ~SGameMtlPair();
    IC int GetMtl0()
    {
        return mtl0;
    }
    IC int GetMtl1()
    {
        return mtl1;
    }
    IC int GetID()
    {
        return ID;
    }
    IC void SetPair(int m0, int m1)
    {
        mtl0 = m0;
        mtl1 = m1;
    }
    IC bool IsPair(int m0, int m1)
    {
        return !!(((mtl0 == m0) && (mtl1 == m1)) || ((mtl0 == m1) && (mtl1 == m0)));
    }
    virtual void Save(IWriter& fs);
    virtual void Load(IReader& fs);
    IC int       GetParent()
    {
        return ID_parent;
    }
#ifdef DEBUG
    LPCSTR dbg_Name();
#endif
};

DEFINE_VECTOR(SGameMtlPair*, GameMtlPairVec, GameMtlPairIt);
class ENGINE_API xrGameMaterialLibraryInterface
{
public:
    xrGameMaterialLibraryInterface();
    virtual ~xrGameMaterialLibraryInterface();
    virtual void          Unload()                            = 0;
    // material routine
    virtual GameMtlIt     GetMaterialIt(LPCSTR name)          = 0;
    virtual GameMtlIt     GetMaterialIt(shared_str& name)     = 0;
    virtual GameMtlIt     GetMaterialItByID(int id)           = 0;
    virtual u32           GetMaterialID(LPCSTR name)          = 0;
    // game
    virtual SGameMtl*     GetMaterialByID(s32 id)             = 0;
    virtual u16           GetMaterialIdx(int ID)              = 0;
    virtual u16           GetMaterialIdx(LPCSTR name)         = 0;
    virtual SGameMtl*     GetMaterialByIdx(u16 idx)           = 0;

    virtual GameMtlIt     FirstMaterial()                     = 0;
    virtual GameMtlIt     LastMaterial()                      = 0;
    virtual u32           CountMaterial()                     = 0;
    // game
    virtual SGameMtlPair* GetMaterialPair(u16 idx0, u16 idx1) = 0;
    virtual GameMtlPairIt FirstMaterialPair()                 = 0;
    virtual GameMtlPairIt LastMaterialPair()                  = 0;

    // IO routines
    virtual void          Load()                              = 0;
    virtual bool          Save()                              = 0;
};
