#pragma once

#include "uiwindow.h"
#include "script_export_space.h"
#include "UIOptionsItem.h"

class CUITabButton;
class CUIButton;

DEF_VECTOR(TABS_VECTOR, CUITabButton*)

class CUITabControl: public CUIWindow, public CUIOptionsItem
{
    typedef CUIWindow inherited;

public:
    CUITabControl();
    virtual ~CUITabControl();

    // options item
    virtual void SetCurrentValue();
    virtual void SaveValue();
    virtual bool IsChanged();

    virtual bool OnKeyboard(int dik, EUIMessages keyboard_action);
    virtual void OnTabChange(int iCur, int iPrev);
    virtual void OnStaticFocusReceive(CUIWindow* pWnd);
    virtual void OnStaticFocusLost(CUIWindow* pWnd);

    // ���������� ������-�������� � ������ �������� ��������
    bool         AddItem(const char* pItemName, const char* pTexName, float x, float y, float width, float height);
    bool         AddItem(CUITabButton* pButton);

    void         RemoveItem(const u32 Index);
    void         RemoveAll();

    virtual void SendMessage(CUIWindow* pWnd, s16 msg, void* pData);

    int          GetActiveIndex()
    {
        return m_iPushedIndex;
    }
    int GetPrevActiveIndex()
    {
        return m_iPrevPushedIndex;
    }
    void      SetNewActiveTab(const int iNewTab);
    const int GetTabsCount() const
    {
        return m_TabsArr.size();
    }

    // ����� ������������� ������������� (���/����)
    IC bool GetAcceleratorsMode() const
    {
        return m_bAcceleratorsEnable;
    }
    void SetAcceleratorsMode(bool bEnable)
    {
        m_bAcceleratorsEnable = bEnable;
    }

    TABS_VECTOR* GetButtonsVector()
    {
        return &m_TabsArr;
    }
    CUIButton*       GetButtonByIndex(int i);
    const shared_str GetCommandName(int i);
    CUIButton*       GetButtonByCommand(const shared_str& n);
    void             ResetTab();

protected:
    // ������ ������ - �������������� ��������
    TABS_VECTOR m_TabsArr;

    // ������� ������� ������. -1 - �� ����, 0 - ������, 1 - ������, � �.�.
    int         m_iPushedIndex;
    int         m_iPrevPushedIndex;

    // ���� ���������� ���������
    u32         m_cGlobalTextColor;
    u32         m_cGlobalButtonColor;

    // ���� ������� �� �������� ��������
    u32         m_cActiveTextColor;
    u32         m_cActiveButtonColor;

    // ���������/��������� ������������ ������������
    bool        m_bAcceleratorsEnable;
    DECLARE_SCRIPT_REGISTER_FUNCTION
};

add_to_type_list(CUITabControl)
#undef script_type_list
#define script_type_list save_type_list(CUITabControl)
