// hit_immunity.h: ����� ��� ��� ��������, ������� ������������
//				   ������������ ���������� ��� ������ ����� �����
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../xrEngine/alife_space.h"
#include "hit_immunity_space.h"

class CHitImmunity
{
public:
    CHitImmunity();
    virtual ~CHitImmunity();

    virtual void  LoadImmunities(LPCSTR section, CInifile* ini);

    virtual float AffectHit(float power, ALife::EHitType hit_type);

protected:
    //������������ �� ������� ����������� ���
    //��� ��������������� ���� �����������
    //(��� �������� �������� � ����������� ��������)
    HitImmunity::HitTypeSVec m_HitTypeK;
};
