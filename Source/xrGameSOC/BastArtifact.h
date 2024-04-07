///////////////////////////////////////////////////////////////
// BastArtifact.h
// BastArtefact - �������� �������
///////////////////////////////////////////////////////////////

#pragma once
#include "artifact.h"
#include "../xrEngine/feel_touch.h"

#include "entity_alive.h"

class SGameMtl;
struct dContact;

DEFINE_VECTOR(CEntityAlive*, ALIVE_LIST, ALIVE_LIST_it);

class CBastArtefact: public CArtefact, public Feel::Touch
{
private:
    typedef CArtefact inherited;

public:
    CBastArtefact(void);
    virtual ~CBastArtefact(void);

    virtual void Load(LPCSTR section);
    virtual void shedule_Update(u32 dt);

    virtual BOOL net_Spawn(CSE_Abstract* DC);
    virtual void net_Destroy();

    virtual void Hit(SHit* pHDS);

    virtual bool Useful() const;

    virtual void feel_touch_new(CObject* O);
    virtual void feel_touch_delete(CObject* O);
    virtual BOOL feel_touch_contact(CObject* O);

    bool         IsAttacking()
    {
        return NULL != m_AttakingEntity;
    }

protected:
    virtual void  UpdateCLChild();

    static void   ObjectContactCallback(bool& do_colide, bool bo1, dContact& c, SGameMtl* /*material_1*/, SGameMtl* /*material_2*/);
    //������������ ������� � ���������
    void          BastCollision(CEntityAlive* pEntityAlive);

    //��������� ���������

    //��������� �������� �������� ����� ���������
    //�������� �������� ��������������
    float         m_fImpulseThreshold;

    float         m_fEnergy;
    float         m_fEnergyMax;
    float         m_fEnergyDecreasePerTime;
    shared_str    m_sParticleName;

    float         m_fRadius;
    float         m_fStrikeImpulse;

    //����, ���� ��� �������� ������� ���
    //� ������ ����� ��������� ������
    bool          m_bStrike;

    //������ ����� ������� � ���� ������������ ���������
    ALIVE_LIST    m_AliveList;
    //��, ��� �� �������
    CEntityAlive* m_pHitedEntity;
    //�� ��� �������
    CEntityAlive* m_AttakingEntity;

public:
    virtual void setup_physic_shell();
};
