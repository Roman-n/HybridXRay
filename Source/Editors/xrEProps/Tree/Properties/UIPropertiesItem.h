#pragma once
class XREPROPS_API UIPropertiesItem: public UITreeItem
{
public:
    UIPropertiesItem(shared_str _Name, UIPropertiesForm* _PropertiesFrom, SLocalizedString _HintText);
    virtual ~UIPropertiesItem();
    PropItem*         PItem;
    UIPropertiesForm* PropertiesFrom;
    void              Draw();
    void              DrawRoot();
    void              DrawItem();
    void              DrawProp();

protected:
    UITreeItem* CreateItem(shared_str _Name, SLocalizedString _HintText) override;

private:
    void RemoveMixed();
};
