// Wound.h: ����� �������� ����
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "..\xrEngine\alife_space.h"
#include "hit_immunity_space.h"

class NET_Packet;

class CWound
{
public:
    CWound(u16 bone_num);
    virtual ~CWound(void);

    //serialization
    virtual void save(NET_Packet& output_packet);
    virtual void load(IReader& input_packet);

    float        TotalSize();
    float        TypeSize(ALife::EHitType hit_type);
    float        BloodSize();

    void         AddHit(float hit_power, ALife::EHitType hit_type);

    //���������� ����
    void         Incarnation(float percent, float min_wound_size);
    u16          GetBoneNum()
    {
        return m_iBoneNum;
    }
    void SetBoneNum(u16 bone_num)
    {
        m_iBoneNum = bone_num;
    }

    u16 GetParticleBoneNum()
    {
        return m_iParticleBoneNum;
    }
    void SetParticleBoneNum(u16 bone_num)
    {
        m_iParticleBoneNum = bone_num;
    }

    const shared_str& GetParticleName()
    {
        return m_sParticleName;
    }
    void SetParticleName(shared_str particle_name)
    {
        m_sParticleName = particle_name;
    }

    void SetDestroy(bool destroy)
    {
        m_bToBeDestroy = destroy;
    }
    bool GetDestroy()
    {
        return m_bToBeDestroy;
    }

    //����� ���������� (��� ������ �����)
    float m_fDropTime;

protected:
    //�������� �� ������� ��������� ����
    u16                      m_iBoneNum;

    //��������, ���� �� ���� ������������ �������
    u16                      m_iParticleBoneNum;
    //��� ����� ��������
    shared_str               m_sParticleName;

    //������ ������������ ����
    HitImmunity::HitTypeSVec m_Wounds;

    bool                     m_bToBeDestroy;
};
