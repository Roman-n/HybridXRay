#include "stdafx.h"

UITreeItem::UITreeItem(shared_str _Name, SLocalizedString _HintText): Name(_Name), HintText(_HintText)
{
    Owner = nullptr;
}

UITreeItem::~UITreeItem()
{
    for (UITreeItem* Item: Items)
    {
        xr_delete(Item);
    }
}

UITreeItem* UITreeItem::AppendItem(const char* _Path, SLocalizedString _HintText, char _PathChar)
{
    VERIFY(_Path && *_Path);
    if (_PathChar && strchr(_Path, _PathChar))
    {
        string_path Name;
        xr_strcpy(Name, _Path);
        strchr(Name, _PathChar)[0] = 0;
        UITreeItem* Item           = FindItem(Name);
        if (!Item)
        {
            Items.push_back(CreateItem(Name, _HintText));
            Item        = Items.back();
            Item->Owner = this;
        }
        return Item->AppendItem(strchr(_Path, _PathChar) + 1, _HintText);
    }
    else
    {
        UITreeItem* Item = FindItem(_Path);
        if (!Item)
        {
            Items.push_back(CreateItem(_Path, _HintText));
            Item        = Items.back();
            Item->Owner = this;
        }
        return Item;
    }
}

UITreeItem* UITreeItem::FindItem(const char* _Path, char _PathChar)
{
    if (_PathChar && strchr(_Path, _PathChar))
    {
        string_path Name;
        xr_strcpy(Name, _Path);
        strchr(Name, _PathChar)[0] = 0;
        UITreeItem* Item           = FindItem(Name);
        if (Item)
        {
            return Item->FindItem(strchr(_Path, _PathChar) + 1);
        }
    }
    else
    {
        shared_str FName = _Path;
        for (UITreeItem* Item: Items)
        {
            if (Item->Name == FName)
            {
                return Item;
            }
        }
    }
    return nullptr;
}

UITreeItem* UITreeItem::CreateItem(shared_str _Name, SLocalizedString _HintText)
{
    return xr_new<UITreeItem>(_Name, _HintText);
}

void UITreeItem::ShowHintIfHovered()
{
    if (HintText.StringEN && HintText.StringRU && ImGui::IsItemHovered())
    {
        if (EditorLocalization == ELocalization::EN)
            ImGui::SetTooltip(HintText.StringEN);
        else
            ImGui::SetTooltip(HintText.StringRU);
    }
}
