#include "stdafx.h"
#pragma hdrstop

#include "bone.h"
#include "envelope.h"
#include "GameMtlLib.h"

void SJointIKData::clamp_by_limits(Fvector& dest_xyz)
{
    switch (type)
    {
        case jtRigid:
            dest_xyz.set(0.f, 0.f, 0.f);
            break;
        case jtJoint:
            clamp(dest_xyz.x, limits[0].limit.x, limits[0].limit.y);
            clamp(dest_xyz.y, limits[1].limit.x, limits[1].limit.y);
            clamp(dest_xyz.z, limits[2].limit.x, limits[2].limit.y);
            break;
        case jtWheel:
            clamp(dest_xyz.x, limits[0].limit.x, limits[0].limit.y);
            dest_xyz.y = 0;
            break;
        case jtSlider:
            dest_xyz.x = 0.f;
            dest_xyz.y = 0.f;
            clamp(dest_xyz.z, limits[1].limit.x, limits[1].limit.y);
            break;
            /*
                case jtWheelXZ:
                    clamp(dest_xyz.x,limits[0].limit.x,limits[0].limit.y);		dest_xyz.y=0;
                break;
                case jtWheelXY:
                    clamp(dest_xyz.x,limits[0].limit.x,limits[0].limit.y);		dest_xyz.z=0;
                break;
                case jtWheelYX:
                    clamp(dest_xyz.y,limits[1].limit.x,limits[1].limit.y);		dest_xyz.z=0;
                break;
                case jtWheelYZ:
                    clamp(dest_xyz.y,limits[1].limit.x,limits[1].limit.y);		dest_xyz.x=0;
                break;
                case jtWheelZX:
                    clamp(dest_xyz.z,limits[2].limit.x,limits[2].limit.y);		dest_xyz.y=0;
                break;
                case jtWheelZY:
                    clamp(dest_xyz.z,limits[2].limit.x,limits[2].limit.y);		dest_xyz.x=0;
                break;
            */
    }
}
#ifndef MASTER_GOLD
void CBone::ShapeScale(const Fvector& _amount)
{
    switch (shape.type)
    {
        case SBoneShape::stBox:
        {
            Fvector amount = _amount;
            //		Fmatrix _IT;_IT.invert(_LTransform());
            //		_IT.transform_dir(amount,_amount);
            //		if (Tools->GetSettings(etfCSParent)) _IT.transform_dir(amount);
            shape.box.m_halfsize.add(amount);
            if (shape.box.m_halfsize.x < EPS)
                shape.box.m_halfsize.x = EPS;
            if (shape.box.m_halfsize.y < EPS)
                shape.box.m_halfsize.y = EPS;
            if (shape.box.m_halfsize.z < EPS)
                shape.box.m_halfsize.z = EPS;
        }
        break;
        case SBoneShape::stSphere:
            shape.sphere.R += _amount.x;
            if (shape.sphere.R < EPS)
                shape.sphere.R = EPS;
            break;
        case SBoneShape::stCylinder:
            shape.cylinder.m_height += _amount.z;
            if (shape.cylinder.m_height < EPS)
                shape.cylinder.m_height = EPS;
            shape.cylinder.m_radius += _amount.x;
            if (shape.cylinder.m_radius < EPS)
                shape.cylinder.m_radius = EPS;
            break;
    }
}

void CBone::BindRotate(const Fvector& _amount)
{
    rest_rotate.add(_amount);
}

void CBone::BindMove(const Fvector& _amount)
{
    rest_offset.add(_amount);
}

bool CBone::Pick(float& dist, const Fvector& S, const Fvector& D, const Fmatrix& parent)
{
    Fvector start, dir;
    Fmatrix M;
    M.mul_43(parent, _LTransform());
    M.invert();
    M.transform_tiny(start, S);
    M.transform_dir(dir, D);
    switch (shape.type)
    {
        case SBoneShape::stBox:
            return shape.box.intersect(start, dir, dist);
        case SBoneShape::stSphere:
            return shape.sphere.intersect(start, dir, dist);
        case SBoneShape::stCylinder:
            return shape.cylinder.intersect(start, dir, dist);
        default:
            Fsphere S;
            S.P.set(0, 0, 0);
            S.R = 0.025f;
            return S.intersect(start, dir, dist);
    }
}

void CBone::BoneMove(const Fvector& _amount)
{
    Fvector amount = _amount;
    switch (IK_data.type)
    {
        case jtSlider:
            amount.x = 0.f;
            amount.y = 0.f;
            rest_i_transform.transform(mot_offset);
            mot_offset.add(amount);
            clamp(mot_offset.z, rest_offset.z + IK_data.limits[0].limit.x, rest_offset.z + IK_data.limits[0].limit.y);
            rest_transform.transform(mot_offset);
            break;
    }
}

void CBone::ClampByLimits()
{
    Fmatrix mBind, mBindI, mLocal, mLocalBP;
    mBind.setXYZi(rest_rotate);
    mBindI.invert(mBind);

    mLocal.setXYZi(mot_rotate);
    mLocalBP.mul(mBindI, mLocal);
    Fvector mot;
    mLocalBP.getXYZi(mot);

    IK_data.clamp_by_limits(mot);

    mLocalBP.setXYZi(mot);
    mLocal.mul(mBind, mLocalBP);
    mLocal.getXYZi(mot_rotate);
}

#endif
#define BONE_VERSION                 0x0002
//------------------------------------------------------------------------------
#define BONE_CHUNK_VERSION           0x0001
#define BONE_CHUNK_DEF               0x0002
#define BONE_CHUNK_BIND_POSE         0x0003
#define BONE_CHUNK_MATERIAL          0x0004
#define BONE_CHUNK_SHAPE             0x0005
#define BONE_CHUNK_IK_JOINT          0x0006
#define BONE_CHUNK_MASS              0x0007
#define BONE_CHUNK_FLAGS             0x0008
#define BONE_CHUNK_IK_JOINT_BREAK    0x0009
#define BONE_CHUNK_IK_JOINT_FRICTION 0x0010

CBone::CBone()
{
    construct();
    flags.zero();
    rest_length = 0;
    SelfID      = -1;
    parent      = 0;

    ResetData();
}

CBone::~CBone() {}

void CBone::ResetData()
{
    IK_data.Reset();
    game_mtl = "default_object";
    shape.Reset();

    mass = 10.f;
    center_of_mass.set(0.f, 0.f, 0.f);
}

void CBone::Save(IWriter& F)
{
#ifdef _LW_EXPORT
    extern void ReplaceSpaceAndLowerCase(shared_str & s);
    ReplaceSpaceAndLowerCase(name);
    ReplaceSpaceAndLowerCase(parent_name);
#endif
    F.open_chunk(BONE_CHUNK_VERSION);
    F.w_u16(BONE_VERSION);
    F.close_chunk();

    F.open_chunk(BONE_CHUNK_DEF);
    F.w_stringZ(name);
    F.w_stringZ(parent_name);
    F.w_stringZ(wmap);
    F.close_chunk();

    F.open_chunk(BONE_CHUNK_BIND_POSE);
    F.w_fvector3(rest_offset);
    F.w_fvector3(rest_rotate);
    F.w_float(rest_length);
    F.close_chunk();

    SaveData(F);
}

void CBone::Load_0(IReader& F)
{
    F.r_stringZ(name);
    xr_strlwr(name);
    F.r_stringZ(parent_name);
    xr_strlwr(parent_name);
    F.r_stringZ(wmap);
    F.r_fvector3(rest_offset);
    F.r_fvector3(rest_rotate);
    rest_length = F.r_float();
    std::swap(rest_rotate.x, rest_rotate.y);
    Reset();
}

void CBone::Load_1(IReader& F)
{
    R_ASSERT(F.find_chunk(BONE_CHUNK_VERSION));
    u16 ver = F.r_u16();

    if ((ver != 0x0001) && (ver != BONE_VERSION))
        return;

    R_ASSERT(F.find_chunk(BONE_CHUNK_DEF));
    F.r_stringZ(name);
    xr_strlwr(name);
    F.r_stringZ(parent_name);
    xr_strlwr(parent_name);
    F.r_stringZ(wmap);

    R_ASSERT(F.find_chunk(BONE_CHUNK_BIND_POSE));
    F.r_fvector3(rest_offset);
    F.r_fvector3(rest_rotate);
    rest_length = F.r_float();

    if (ver == 0x0001)
        std::swap(rest_rotate.x, rest_rotate.y);

    LoadData(F);
}

void CBone::SaveData(IWriter& F)
{
    F.open_chunk(BONE_CHUNK_DEF);
    F.w_stringZ(name);
    F.close_chunk();

    F.open_chunk(BONE_CHUNK_MATERIAL);
    F.w_stringZ(game_mtl);
    F.close_chunk();

    F.open_chunk(BONE_CHUNK_SHAPE);
    F.w(&shape, sizeof(SBoneShape));
    F.close_chunk();

    F.open_chunk(BONE_CHUNK_FLAGS);
    F.w_u32(IK_data.ik_flags.get());
    F.close_chunk();

    F.open_chunk(BONE_CHUNK_IK_JOINT);
    F.w_u32(IK_data.type);
    F.w(IK_data.limits, sizeof(SJointLimit) * 3);
    F.w_float(IK_data.spring_factor);
    F.w_float(IK_data.damping_factor);
    F.close_chunk();

    F.open_chunk(BONE_CHUNK_IK_JOINT_BREAK);
    F.w_float(IK_data.break_force);
    F.w_float(IK_data.break_torque);
    F.close_chunk();

    F.open_chunk(BONE_CHUNK_IK_JOINT_FRICTION);
    F.w_float(IK_data.friction);
    F.close_chunk();

    F.open_chunk(BONE_CHUNK_MASS);
    F.w_float(mass);
    F.w_fvector3(center_of_mass);
    F.close_chunk();
}

void CBone::LoadData(IReader& F)
{
    R_ASSERT(F.find_chunk(BONE_CHUNK_DEF));
    F.r_stringZ(name);
    xr_strlwr(name);

    R_ASSERT(F.find_chunk(BONE_CHUNK_MATERIAL));
    F.r_stringZ(game_mtl);

    R_ASSERT(F.find_chunk(BONE_CHUNK_SHAPE));
    F.r(&shape, sizeof(SBoneShape));

    if (F.find_chunk(BONE_CHUNK_FLAGS))
        IK_data.ik_flags.assign(F.r_u32());

    R_ASSERT(F.find_chunk(BONE_CHUNK_IK_JOINT));
    IK_data.type = (EJointType)F.r_u32();
    F.r(IK_data.limits, sizeof(SJointLimit) * 3);
    IK_data.spring_factor  = F.r_float();
    IK_data.damping_factor = F.r_float();

    if (F.find_chunk(BONE_CHUNK_IK_JOINT_BREAK))
    {
        IK_data.break_force  = F.r_float();
        IK_data.break_torque = F.r_float();
    }

    if (F.find_chunk(BONE_CHUNK_IK_JOINT_FRICTION))
    {
        IK_data.friction = F.r_float();
    }

    if (F.find_chunk(BONE_CHUNK_MASS))
    {
        mass = F.r_float();
        F.r_fvector3(center_of_mass);
    }
}

void CBone::CopyData(CBone* bone)
{
    game_mtl       = bone->game_mtl;
    shape          = bone->shape;
    IK_data        = bone->IK_data;
    mass           = bone->mass;
    center_of_mass = bone->center_of_mass;
}

void SJointIKData::Export(IWriter& F)
{
    F.w_u32(type);
    for (int k = 0; k < 3; k++)
    {
        // Kostya Slipchenko say:
        // направление вращения в ОДЕ отличается от направления вращение в X-Ray
        // поэтому меняем знак у лимитов
        // F.w_float(_min(-limits[k].limit.x,-limits[k].limit.y)); // min (swap special for ODE)
        // F.w_float(_max(-limits[k].limit.x,-limits[k].limit.y)); // max (swap special for ODE)

        if (_min(-limits[k].limit.x, -limits[k].limit.y) != -limits[k].limit.y)
            Msg("! ..Warning: _min(-limits[k].limit.x, -limits[k].limit.y) != -limits[k].limit.y");

        if (_max(-limits[k].limit.x, -limits[k].limit.y) != -limits[k].limit.x)
            Msg("! ..Warning: _max(-limits[k].limit.x,-limits[k].limit.y) != -limits[k].limit.x");

        F.w_float(-limits[k].limit.y);   // min (swap special for ODE)
        F.w_float(-limits[k].limit.x);   // max (swap special for ODE)

        F.w_float(limits[k].spring_factor);
        F.w_float(limits[k].damping_factor);
    }
    F.w_float(spring_factor);
    F.w_float(damping_factor);

    F.w_u32(ik_flags.get());
    F.w_float(break_force);
    F.w_float(break_torque);

    F.w_float(friction);
}

bool SJointIKData::Import(IReader& F, u16 vers)
{
    type = (EJointType)F.r_u32();
    F.r(limits, sizeof(SJointLimit) * 3);
    spring_factor  = F.r_float();
    damping_factor = F.r_float();
    ik_flags.flags = F.r_u32();
    break_force    = F.r_float();
    break_torque   = F.r_float();
    if (vers > 0)
    {
        friction = F.r_float();
    }
    return true;
}
