//=============================================================================
//  Filename:   UIColorAnimatorWrapper.h
//	Created by Roman E. Marchenko, vortex@gsc-game.kiev.ua
//	Copyright 2004. GSC Game World
//	---------------------------------------------------------------------------
//  ��������� ��� LightAnimLibrary, � ������� ����������� ����� �������
//	������ � ���������, � ��������� ������ ������� ����������� ��� UI: ���
//	��� UI ���������� �� �� ����� �����, �� ������� ������� ������������
//	Device->fTimeDelta ������ - �������� ����������� ����������. ����������
//	��������� ������� ����� ����� ���������.
//=============================================================================
#pragma once

//////////////////////////////////////////////////////////////////////////

class CLAItem;

//////////////////////////////////////////////////////////////////////////

class CUIColorAnimatorWrapper
{
public:
    // �������� ����� �������� ����� �������� �������.
    // ����� ������ �� ������ ����������� Reset()
    // ��� ������������ ������ 1 ����� �������� Cyclic(false);

    CUIColorAnimatorWrapper();
    ~CUIColorAnimatorWrapper(){};
    // colorToModify - ��������� �� ���� ������� ������
    // animationName - ��� �������� ��������
    explicit CUIColorAnimatorWrapper(const shared_str& animationName);
    //						CUIColorAnimatorWrapper	(const shared_str &animationName, u32 *colorToModify);
    CUIColorAnimatorWrapper(u32* colorToModify);

    // ���������� ����� �������� ��������
    void SetColorAnimation(const shared_str& animationName);
    void SetColorToModify(u32* colorToModify);
    // ������� ������� ���������� ����������� ��������� � Update �������, ��� ���������� ��������
    void Update();
    void Cyclic(bool cyclic)
    {
        isCyclic = cyclic;
    }
    void Reset();
    u32  GetColor() const
    {
        return currColor;
    }
    int LastFrame() const
    {
        return currFrame;
    }
    int  TotalFrames() const;
    bool Done() const
    {
        return isDone;
    }
    void SetDone(bool value)
    {
        isDone = value;
    }
    void Reverese(bool value);
    void GoToEnd();

private:
    // ���������� ��������
    CLAItem* colorAnimation;
    // ���������� ���������� �������� ����������� ������� ����
    float    prevGlobalTime;
    // ����� ��������� � ������ ��������
    float    animationTime;
    // ����������� ����������� ������������
    bool     isCyclic;
    // ���� ������� �� ����� �������� ���������� ��� �����
    u32*     color;
    // ������� ���� ��������
    u32      currColor;
    // ������� ���� ��������
    int      currFrame;
    // �������� �����������?
    bool     isDone;
    // �������� �������������� � ����������� �������? �� ��������� ��� ����������� ��������
    bool     reverse;
    // ����������� ��� ��������� ������� ��� ����������� ������������
    float    kRev;

public:
    CLAItem* GetAnimation() const
    {
        return colorAnimation;
    }
};
