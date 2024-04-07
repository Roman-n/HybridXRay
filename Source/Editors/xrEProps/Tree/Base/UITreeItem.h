#pragma once
class XREPROPS_API UITreeItem
{
public:
    UITreeItem(shared_str _Name, SLocalizedString _HintText);
    virtual ~UITreeItem();

    UITreeItem*            AppendItem(const char* _Path, SLocalizedString _HintText = {}, char _PathChar = '\\');
    UITreeItem*            FindItem(const char* _Path, char _PathChar = '\\');
    xr_vector<UITreeItem*> Items;
    UITreeItem*            Owner;
    shared_str             Name;
    SLocalizedString       HintText;

protected:
    virtual UITreeItem* CreateItem(shared_str _Name, SLocalizedString _HintText);
    void                ShowHintIfHovered();
};
