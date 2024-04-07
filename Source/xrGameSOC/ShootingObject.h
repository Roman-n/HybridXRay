//////////////////////////////////////////////////////////////////////
// ShootingObject.h: ��������� ��� ��������� ���������� ��������
//					 (������ � ���������� �������)
//					 ������������ ����� �����, ������ ��������
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../xrEngine/alife_space.h"
#include "../xrEngine/render.h"

class CCartridge;
class CParticlesObject;
class IRender_Sector;

extern const Fvector zero_vel;

#define WEAPON_MATERIAL_NAME "objects\\bullet"

class CShootingObject
{
protected:
    CShootingObject(void);
    virtual ~CShootingObject(void);

    void    reinit();
    void    reload(LPCSTR section){};
    void    Load(LPCSTR section);

    Fvector m_vCurrentShootDir;
    Fvector m_vCurrentShootPos;
    //ID ��������� ������� ���������� ��������
    u16     m_iCurrentParentID;

    //////////////////////////////////////////////////////////////////////////
    // Fire Params
    //////////////////////////////////////////////////////////////////////////

protected:
    virtual void LoadFireParams(LPCSTR section, LPCSTR prefix);
    virtual bool SendHitAllowed(CObject* pUser);
    virtual void FireBullet(const Fvector& pos, const Fvector& dir, float fire_disp, const CCartridge& cartridge, u16 parent_id, u16 weapon_id, bool send_hit);

    virtual void FireStart();
    virtual void FireEnd();

public:
    IC BOOL IsWorking() const
    {
        return bWorking;
    }
    virtual BOOL ParentMayHaveAimBullet()
    {
        return FALSE;
    }
    virtual BOOL ParentIsActor()
    {
        return FALSE;
    }

protected:
    // Weapon fires now
    bool     bWorking;

    float    fTimeToFire;
    Fvector4 fvHitPower;
    //float					fHitPower;
    float    fHitImpulse;

    //�������� ������ ���� �� ������
    float    m_fStartBulletSpeed;
    //������������ ���������� ��������
    float    fireDistance;

    //����������� �� ����� ��������
    float    fireDispersionBase;

    //������� �������, �������������� �� �������
    float    fTime;

protected:
    //��� ���������, ���� ��� ����� ����������� ������� �������������
    //������
    float m_fMinRadius;
    float m_fMaxRadius;

    //////////////////////////////////////////////////////////////////////////
    // Lights
    //////////////////////////////////////////////////////////////////////////

protected:
    Fcolor    light_base_color;
    float     light_base_range;
    Fcolor    light_build_color;
    float     light_build_range;
    ref_light light_render;
    float     light_var_color;
    float     light_var_range;
    float     light_lifetime;
    u32       light_frame;
    float     light_time;
    //��������� ��������� �� ����� ��������
    bool      m_bLightShotEnabled;

protected:
    void         Light_Create();
    void         Light_Destroy();

    void         Light_Start();
    void         Light_Render(const Fvector& P);

    virtual void LoadLights(LPCSTR section, LPCSTR prefix);
    virtual void RenderLight();
    virtual void UpdateLight();
    virtual void StopLight();

    //////////////////////////////////////////////////////////////////////////
    // ����������� �������
    //////////////////////////////////////////////////////////////////////////

protected:
    //������� ������������� �������
    virtual const Fvector& get_CurrentFirePoint() = 0;
    virtual const Fmatrix& get_ParticlesXFORM()   = 0;
    virtual void           ForceUpdateFireParticles(){};

    ////////////////////////////////////////////////
    //����� ������� ��� ������ � ���������� ������
    virtual void           StartParticles(CParticlesObject*& pParticles, LPCSTR particles_name, const Fvector& pos, const Fvector& vel = zero_vel, bool auto_remove_flag = false);
    virtual void           StopParticles(CParticlesObject*& pParticles);
    virtual void           UpdateParticles(CParticlesObject*& pParticles, const Fvector& pos, const Fvector& vel = zero_vel);

    virtual void           LoadShellParticles(LPCSTR section, LPCSTR prefix);
    virtual void           LoadFlameParticles(LPCSTR section, LPCSTR prefix);

    ////////////////////////////////////////////////
    //������������� ������� ��� ���������
    //�������� ����
    virtual void           StartFlameParticles();
    virtual void           StopFlameParticles();
    virtual void           UpdateFlameParticles();

    //�������� ����
    virtual void           StartSmokeParticles(const Fvector& play_pos, const Fvector& parent_vel);

    //�������� ������ �� ����
    virtual void           StartShotParticles();

    //�������� �����
    virtual void           OnShellDrop(const Fvector& play_pos, const Fvector& parent_vel);

protected:
    //��� ��������� ��� �����
    shared_str m_sShellParticles;

public:
    Fvector vLoadedShellPoint;
    float   m_fPredBulletTime;
    float   m_fTimeToAim;
    BOOL    m_bUseAimBullet;

protected:
    //��� ��������� ��� ����
    shared_str        m_sFlameParticlesCurrent;
    //��� �������� 1� � 2� ����� ��������
    shared_str        m_sFlameParticles;
    //������ ��������� ����
    CParticlesObject* m_pFlameParticles;

    //��� ��������� ��� ����
    shared_str        m_sSmokeParticlesCurrent;
    shared_str        m_sSmokeParticles;

    //��� ��������� ����� �� ����
    shared_str        m_sShotParticles;
};
