//////////////////////////////////////////////////////////////////////////
// relation_registry_defs.h:	������ ��� �������� ������ �� ��������� ��������� �
//								������ ����������
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "../xrEngine/object_interfaces.h"

//���������, ����������� ��������� ������ ��������� � ������� ��� � �����������
struct SRelation
{
    SRelation();
    ~SRelation();
    CHARACTER_GOODWILL Goodwill() const
    {
        return m_iGoodwill;
    };
    void SetGoodwill(CHARACTER_GOODWILL new_goodwill)
    {
        m_iGoodwill = new_goodwill;
    };

private:
    //���������������
    CHARACTER_GOODWILL m_iGoodwill;
};

DEFINE_MAP(u16, SRelation, PERSONAL_RELATION_MAP, PERSONAL_RELATION_MAP_IT);
DEFINE_MAP(CHARACTER_COMMUNITY_INDEX, SRelation, COMMUNITY_RELATION_MAP, COMMUNITY_RELATION_MAP_IT);

//���������, ���������� ��� ������� ��������� � ����
struct RELATION_DATA: public IPureSerializeObject<IReader, IWriter>
{
    virtual void           clear();

    virtual void           load(IReader&);
    virtual void           save(IWriter&);

    //������ ���������
    PERSONAL_RELATION_MAP  personal;
    //��������� � �������������
    COMMUNITY_RELATION_MAP communities;
};
