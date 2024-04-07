#include "stdafx.h"
#include "xrGameMaterialLibraryInterface.h"
SGameMtl::SGameMtl()
{
    ID     = -1;
    m_Name = "unknown";
    Flags.zero();
    // factors
    fFlotationFactor       = 1.f;
    fShootFactor           = 0.f;
    fShootFactorMP         = 0.f;
    fBounceDamageFactor    = 1.f;
    fInjuriousSpeed        = 0.f;
    fVisTransparencyFactor = 0.f;
    fSndOcclusionFactor    = 0.f;
    // physics
    fPHFriction            = 1.f;
    fPHDamping             = 1.f;
    fPHSpring              = 1.f;
    fPHBounceStartVelocity = 0.f;
    fPHBouncing            = 0.1f;
    fDensityFactor         = 0.0f;
}

SGameMtl::~SGameMtl() {}

void DestroySounds(SoundVec& lst)
{
    for (SoundIt it = lst.begin(); lst.end() != it; ++it)
        it->destroy();
}
/*
void DestroyMarks(ShaderVec& lst)
{
    for (ShaderIt it=lst.begin(); lst.end() != it; ++it)
        it->destroy();
}
*/

void DestroyPSs(PSVec& lst)
{
    //	for (PSIt it=lst.begin(); lst.end() != it; ++it)
    //		Device->Resources->Delete(*it);
}

void CreateSounds(SoundVec& lst, LPCSTR buf)
{
    string128 tmp;
    int       cnt = _GetItemCount(buf);
    R_ASSERT(cnt <= GAMEMTL_SUBITEM_COUNT + 2);
    lst.resize(cnt);
    for (int k = 0; k < cnt; ++k)
        lst[k].create(_GetItem(buf, k, tmp), st_Effect, sg_SourceType);
}
/*
void CreateMarks(ShaderVec& lst, LPCSTR buf)
{
    string256	tmp;
    int cnt		=_GetItemCount(buf);	R_ASSERT(cnt<=GAMEMTL_SUBITEM_COUNT);
    ref_shader	s;
    for (int k=0; k<cnt; ++k)
    {
        s.create		("effects\\wallmark",_GetItem(buf,k,tmp));
        lst.push_back	(s);
    }
}
*/
void CreateMarks(IWallMarkArray* pMarks, LPCSTR buf)
{
    string256 tmp;
    int       cnt = _GetItemCount(buf);
    R_ASSERT(cnt <= GAMEMTL_SUBITEM_COUNT);
    for (int k = 0; k < cnt; ++k)
        pMarks->AppendMark(_GetItem(buf, k, tmp));
}

void CreatePSs(PSVec& lst, LPCSTR buf)
{
    string256 tmp;
    int       cnt = _GetItemCount(buf);
    R_ASSERT(cnt <= GAMEMTL_SUBITEM_COUNT);
    for (int k = 0; k < cnt; ++k)
        lst.push_back(_GetItem(buf, k, tmp));
}

SGameMtlPair::~SGameMtlPair()
{
    // destroy all media
    DestroySounds(BreakingSounds);
    DestroySounds(StepSounds);
    DestroySounds(CollideSounds);
    DestroyPSs(CollideParticles);
    //	DestroyMarks	(CollideMarks);
    // RenderFactory->DestroyGameMtlPair(m_pCollideMarks);
    // m_pCollideMarks->
}

void SGameMtlPair::Load(IReader& fs)
{
    shared_str buf;

    R_ASSERT(fs.find_chunk(GAMEMTLPAIR_CHUNK_PAIR));
    mtl0      = fs.r_u32();
    mtl1      = fs.r_u32();
    ID        = fs.r_u32();
    ID_parent = fs.r_u32();
    OwnProps.assign(fs.r_u32());

    R_ASSERT(fs.find_chunk(GAMEMTLPAIR_CHUNK_BREAKING));
    fs.r_stringZ(buf);
    CreateSounds(BreakingSounds, *buf);

    R_ASSERT(fs.find_chunk(GAMEMTLPAIR_CHUNK_STEP));
    fs.r_stringZ(buf);
    CreateSounds(StepSounds, *buf);

    R_ASSERT(fs.find_chunk(GAMEMTLPAIR_CHUNK_COLLIDE));
    fs.r_stringZ(buf);
    CreateSounds(CollideSounds, *buf);
    fs.r_stringZ(buf);
    CreatePSs(CollideParticles, *buf);
    fs.r_stringZ(buf);
    // CreateMarks			(CollideMarks,*buf);
    CreateMarks(&*m_pCollideMarks, *buf);
}

#if 0
void SGameMtlPair::Load(IReader& fs)
{
    shared_str				buf;

    R_ASSERT(fs.find_chunk(GAMEMTLPAIR_CHUNK_PAIR));
    mtl0 = fs.r_u32();
    mtl1 = fs.r_u32();
    ID = fs.r_u32();
    ID_parent = fs.r_u32();
    u32 own_mask = fs.r_u32();
    if (GAMEMTL_NONE_ID == ID_parent) OwnProps.one();
    else							OwnProps.assign(own_mask);

    R_ASSERT(fs.find_chunk(GAMEMTLPAIR_CHUNK_BREAKING));
    fs.r_stringZ(buf); 	BreakingSounds = buf.size() ? *buf : "";

    R_ASSERT(fs.find_chunk(GAMEMTLPAIR_CHUNK_STEP));
    fs.r_stringZ(buf);	StepSounds = buf.size() ? *buf : "";

    R_ASSERT(fs.find_chunk(GAMEMTLPAIR_CHUNK_COLLIDE));
    fs.r_stringZ(buf);	CollideSounds = buf.size() ? *buf : "";
    fs.r_stringZ(buf);	CollideParticles = buf.size() ? *buf : "";
    fs.r_stringZ(buf);	CollideMarks = buf.size() ? *buf : "";
    R_ASSERT(0);
}

#endif
void SGameMtlPair::Save(IWriter& fs)
{
    R_ASSERT(0);
}
void SGameMtl::Load(IReader& fs)
{
    R_ASSERT(fs.find_chunk(GAMEMTL_CHUNK_MAIN));
    ID = fs.r_u32();
    fs.r_stringZ(m_Name);

    if (fs.find_chunk(GAMEMTL_CHUNK_DESC))
    {
        fs.r_stringZ(m_Desc);
    }

    R_ASSERT(fs.find_chunk(GAMEMTL_CHUNK_FLAGS));
    Flags.assign(fs.r_u32());

    R_ASSERT(fs.find_chunk(GAMEMTL_CHUNK_PHYSICS));
    fPHFriction            = fs.r_float();
    fPHDamping             = fs.r_float();
    fPHSpring              = fs.r_float();
    fPHBounceStartVelocity = fs.r_float();
    fPHBouncing            = fs.r_float();

    R_ASSERT(fs.find_chunk(GAMEMTL_CHUNK_FACTORS));
    fShootFactor           = fs.r_float();
    fBounceDamageFactor    = fs.r_float();
    fVisTransparencyFactor = fs.r_float();
    fSndOcclusionFactor    = fs.r_float();

    if (fs.find_chunk(GAMEMTL_CHUNK_FACTORS_MP))
        fShootFactorMP = fs.r_float();
    else
        fShootFactorMP = fShootFactor;

    if (fs.find_chunk(GAMEMTL_CHUNK_FLOTATION))
        fFlotationFactor = fs.r_float();

    if (fs.find_chunk(GAMEMTL_CHUNK_INJURIOUS))
        fInjuriousSpeed = fs.r_float();

    if (fs.find_chunk(GAMEMTL_CHUNK_DENSITY))
        fDensityFactor = fs.r_float();
}

void SGameMtl::Save(IWriter& fs)
{
    R_ASSERT(0);
}

xrGameMaterialLibraryInterface::xrGameMaterialLibraryInterface() {}

xrGameMaterialLibraryInterface::~xrGameMaterialLibraryInterface() {}
#ifdef DEBUG
LPCSTR SGameMtlPair::dbg_Name()
{
    static string256 nm;
    SGameMtl*        M0 = GameMaterialLibrary->GetMaterialByID(GetMtl0());
    SGameMtl*        M1 = GameMaterialLibrary->GetMaterialByID(GetMtl1());
    xr_sprintf(nm, sizeof(nm), "Pair: %s - %s", *M0->m_Name, *M1->m_Name);
    return nm;
}
#endif

SGameMtlPair::SGameMtlPair(xrGameMaterialLibraryInterface* owner)
{
#ifdef _EDITOR
    m_EditParent  = false;
    m_EditCommand = false;
#endif
#ifndef GM_NON_GAME
    // m_pCollideMarks = RenderFactory->CreateGameMtlPair();
#endif   //	GM_NON_GAME
    mtl0      = -1;
    mtl1      = -1;
    ID        = -1;
    ID_parent = -1;
    m_Owner   = owner;
    OwnProps.one();
}
