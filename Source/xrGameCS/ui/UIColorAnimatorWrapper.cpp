//=============================================================================
//  Filename:   UIColorAnimatorWrapper.cpp
//	Created by Roman E. Marchenko, vortex@gsc-game.kiev.ua
//	Copyright 2004. GSC Game World
//	---------------------------------------------------------------------------
//  ��������� ��� LightAnimLibrary, � ������� ����������� ����� �������
//	������ � ���������, � ��������� ������ ������� ����������� ��� UI: ���
//	��� UI ���������� �� �� ����� �����, �� ������� ������� ������������
//	Device->fTimeDelta ������ - �������� ����������� ����������. ����������
//	��������� ������� ����� ����� ���������.
//=============================================================================

#include "stdafx.h"
#include "UIColorAnimatorWrapper.h"
#include "../../xrEngine/LightAnimLibrary.h"

//////////////////////////////////////////////////////////////////////////

CUIColorAnimatorWrapper::CUIColorAnimatorWrapper(): colorAnimation(NULL), animationTime(0), color(NULL), isDone(false), reverse(false), kRev(0.0f)
{
    prevGlobalTime = Device->dwTimeContinual / 1000.0f;
    currColor      = 0xffff0000;
}

CUIColorAnimatorWrapper::CUIColorAnimatorWrapper(u32* colorToModify): colorAnimation(NULL), animationTime(0), color(NULL), isDone(false), reverse(false), kRev(0.0f)
{
    prevGlobalTime = Device->dwTimeContinual / 1000.0f;
    color          = colorToModify;
    currColor      = 0xffff0000;
}
//////////////////////////////////////////////////////////////////////////

CUIColorAnimatorWrapper::CUIColorAnimatorWrapper(const shared_str& animationName): colorAnimation(LALib.FindItem(*animationName)), animationTime(0), color(NULL), isDone(false), reverse(false), kRev(0.0f)
{
    VERIFY(colorAnimation);
    prevGlobalTime = Device->dwTimeContinual / 1000.0f;
    currColor      = 0xffff0000;
}

void CUIColorAnimatorWrapper::SetColorAnimation(const shared_str& animationName)
{
    if (animationName.size() != 0)
    {
        colorAnimation = LALib.FindItem(*animationName);
        R_ASSERT2(colorAnimation, *animationName);
    }
    else
    {
        colorAnimation = NULL;
    }
}

//////////////////////////////////////////////////////////////////////////

void CUIColorAnimatorWrapper::SetColorToModify(u32* colorToModify)
{
    color = colorToModify;
}

//////////////////////////////////////////////////////////////////////////

void CUIColorAnimatorWrapper::Update()
{
    if (colorAnimation && !isDone)
    {
        if (!isCyclic)
        {
            if (animationTime < (colorAnimation->iFrameCount / colorAnimation->fFPS))
            {
                currColor = colorAnimation->CalculateBGR(std::abs(animationTime - kRev), currFrame);
                //Msg("name: %s, color: %x, frame: %d", *colorAnimation->cName,currColor, currFrame);
                currColor = color_rgba(color_get_B(currColor), color_get_G(currColor), color_get_R(currColor), color_get_A(currColor));
                // ������� �����
                animationTime += Device->dwTimeContinual / 1000.0f - prevGlobalTime;
            }
            else
            {
                // � ����� ������ (��� ����� ���) ��������� ������ ������ ���� ��������� ���� ��������
                currColor = colorAnimation->CalculateBGR((colorAnimation->iFrameCount - 1) / colorAnimation->fFPS - kRev, currFrame);
                currColor = color_rgba(color_get_B(currColor), color_get_G(currColor), color_get_R(currColor), color_get_A(currColor));
                // ���������� ����� ��������
                isDone    = true;
            }
        }
        else
        {
            currColor = colorAnimation->CalculateBGR(Device->dwTimeContinual / 1000.0f, currFrame);
            currColor = color_rgba(color_get_B(currColor), color_get_G(currColor), color_get_R(currColor), color_get_A(currColor));
        }

        if (color)
        {
            *color = currColor;
        }
    }

    prevGlobalTime = Device->dwTimeContinual / 1000.0f;
}

//////////////////////////////////////////////////////////////////////////

void CUIColorAnimatorWrapper::Reset()
{
    prevGlobalTime = Device->dwTimeContinual / 1000.0f;
    animationTime  = 0;
    isDone         = false;
}

//////////////////////////////////////////////////////////////////////////

int CUIColorAnimatorWrapper::TotalFrames() const
{
    if (colorAnimation)
    {
        return colorAnimation->iFrameCount;
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////

void CUIColorAnimatorWrapper::Reverese(bool value)
{
    reverse = value;

    if (value)
    {
        kRev = (colorAnimation->iFrameCount - 1) / colorAnimation->fFPS;
    }
    else
    {
        kRev = 0.0f;
    }

    if (!Done())
    {
        animationTime = colorAnimation->iFrameCount / colorAnimation->fFPS - animationTime;
    }
}

void CUIColorAnimatorWrapper::GoToEnd()
{
    prevGlobalTime  = Device->dwTimeContinual / 1000.0f;
    this->currFrame = colorAnimation->iFrameCount;
    animationTime   = colorAnimation->iFrameCount / colorAnimation->fFPS;
    this->isDone    = false;
}
