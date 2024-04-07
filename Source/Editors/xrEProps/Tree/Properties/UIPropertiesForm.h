#pragma once

class XREPROPS_API UIPropertiesForm: public xrUI
{
    friend class UIPropertiesItem;

public:
    UIPropertiesForm();
    ~UIPropertiesForm() override;

    void      Draw() override;
    void      AssignItems(PropItemVec& items);
    PropItem* FindItem(const char* path);
    PropItem* FindItemOfName(shared_str name);
    void      ClearProperties();
    void      SetReadOnly(bool enable)
    {
        m_Flags.set(plReadOnly, enable);
    }
    bool IsModified() const
    {
        return m_bModified;
    }
    void setModified(bool val)
    {
        m_bModified = val;
    }
    bool Empty() const
    {
        return m_Items.empty();
    }
    void SetModifiedEvent(TOnModifiedEvent modif = 0)
    {
        OnModifiedEvent = modif;
    }

    enum
    {
        plReadOnly = (1 << 0),
    };

    Flags32 m_Flags;
    void    SetFocusOnMe();
    bool    IsReadOnly() const
    {
        return m_Flags.is(plReadOnly);
    }

private:
    PropItemVec      m_Items;
    PropItem*        m_EditChooseValue;
    PropItem*        m_EditTextureValue;
    PropItem*        m_EditShortcutValue;

    TOnModifiedEvent OnModifiedEvent;
    PropItem*        m_EditTextValue;

    char*            m_EditTextValueData;
    char*            m_EditTextValueInitial;
    int              m_EditTextValueDataSize;

    void             DrawEditText();
    int              DrawEditText_Callback(ImGuiInputTextCallbackData* data);

    GameTypeChooser  m_EditGameTypeChooser;
    PropItem*        m_EditGameTypeValue;
    bool             m_bSingle;
    bool             m_bDM;
    bool             m_bTDM;
    bool             m_bAH;
    bool             m_bCTA;

    void             DrawEditGameType();
    bool             m_bModified;
    void             Modified()
    {
        m_bModified = true;
        if (!OnModifiedEvent.empty())
            OnModifiedEvent();
    }

    UIPropertiesItem m_Root;
};
