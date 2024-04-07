/////////////////////////////////////////////////////
//
// PdaMsg.h - ��������� ���������� ��� ������ PDA
//
/////////////////////////////////////////////////////

#pragma once
#include "../xrEngine/alife_space.h"
#include "pda_space.h"

//��������� ��� �������� ��������� PDA,
//������������ ��� ������� �����
typedef struct tagSPdaMessage
{
    EPdaMsg         msg;

    //true ���� �� �������� ���������
    //� false ���� �� ��� ��������
    bool            receive;

    //true, ���� ��������� - ������
    //� false, ���� �����
    bool            question;

    // ���������
    shared_str      info_id;

    //����� ���������/�������� ���������
    ALife::_TIME_ID time;

} SPdaMessage;

//���������� � ��������� ���������� �� PDA � �� ����� �������
struct TALK_CONTACT_DATA
{
    TALK_CONTACT_DATA(): id(u16(-1)), time(0){};
    TALK_CONTACT_DATA(u16 contact_id, ALife::_TIME_ID contact_time): id(contact_id), time(contact_time){};
    //����� ��������
    ALife::_TIME_ID time;
    //id ��������� � ������� ��������
    u16             id;
};

DEFINE_VECTOR(TALK_CONTACT_DATA, TALK_CONTACT_VECTOR, TALK_CONTACT_VECTOR_IT);
