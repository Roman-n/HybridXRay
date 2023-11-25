#include "stdafx.h"
#include "UIGroupTool.h"

#include <random>

UIGroupTool::UIGroupTool()
{
    m_ChooseGroup = false;
    m_selPercent  = true;
}

UIGroupTool::~UIGroupTool() {}
void UIGroupTool::Draw()
{
    // ------------------------------------------------------------------------------------------------------ //
    ImGui::Separator();
    {
        ImGui::BulletText("Commands"_RU >> u8"Правка", ImGuiDir_Left);
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::BeginPopupContextItem("Commands", 1))
        {
            if (ImGui::MenuItem("Group"_RU >> u8"Группировать"))
            {
                ParentTools->GroupObjects();
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::MenuItem("Ungroup"_RU >> u8"Разгруппировать"))
            {
                ParentTools->UngroupObjects();
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::Separator();
            if (ImGui::MenuItem("Make Thumbnail"_RU >> u8"Сделать миниатюру"))
            {
                ParentTools->MakeThumbnail();
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::Separator();
            if (ImGui::MenuItem("Save As ..."_RU >> u8"Сохранить как ..."))
            {
                ParentTools->SaveSelectedObject();
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::EndPopup();
        }
        ImGui::OpenPopupOnItemClick("Commands", 0);
    }
    ImGui::Separator();
    // ------------------------------------------------------------------------------------------------------ //
    if (ImGui::CollapsingHeader(("  Current Object"_RU >> u8"  Библиотека"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTableFlags_NoBordersInBody))
    {
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SameLine(0, 10);
        ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "");
        ImGui::BeginGroup();
        {
            ImGui::SetNextItemWidth(-1);
            float size = float(ImGui::CalcItemWidth());
            {
                if (ImGui::Button("Select ..."_RU >> u8"Выбрать ...", ImVec2(size / 2, 0)))
                {
                    UIChooseForm::SelectItem(smGroup, 1, m_Current.c_str());
                    m_ChooseGroup = true;
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SameLine(0, 2);
                if (ImGui::Button("Reload Refs"_RU >> u8"Обновить", ImVec2(size / 2, 0)))
                {
                    ParentTools->ReloadRefsSelectedObject();
                    // bForceInitListBox = TRUE;
                    Tools->UpdateProperties(TRUE);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            }
            ImGui::Text("Current:  %s"_RU >> u8"Текущий:  %s", m_Current.c_str() ? m_Current.c_str() : "");
        }
        ImGui::Separator();
        ImGui::EndGroup();
    }
    // ------------------------------------------------------------------------------------------------------ //
    if (ImGui::CollapsingHeader(("  Reference Select"_RU >> u8"  Редактор выделенного"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTableFlags_NoBordersInBody))
    {
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SameLine(0, 10);
        ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "");
        ImGui::BeginGroup();
        {
            ImGui::Text("Select by Current: "_RU >> u8"Выбрать по текущему:      ");
            ImGui::SameLine(0, 10);
            if (ImGui::Button(" + "))
            {
                SelByRefObject(true);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SameLine(0, 8);
            if (ImGui::Button(" - "))
            {
                SelByRefObject(false);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::Text("Select by Selected:"_RU >> u8"Выбрать по выбранному: ");
            ImGui::SameLine();
            if (ImGui::Button("=%"))
            {
                MultiSelByRefObject(true);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SameLine();
            if (ImGui::Button("+%"))
            {
                MultiSelByRefObject(false);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(-ImGui::GetTextLineHeight() - 8);
            ImGui::DragFloat("%", &m_selPercent, 1, 0, 100, "%.1f");
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }
        ImGui::Separator();
        ImGui::EndGroup();
    }
    // ------------------------------------------------------------------------------------------------------ //
    if (ImGui::CollapsingHeader(("  Pivot Alignment"_RU >> u8"  Выравнивание по оси"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTableFlags_NoBordersInBody))
    {
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SameLine(0, 10);
        ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "");
        ImGui::BeginGroup();
        {
            if (ImGui::Button("Center To Group"_RU >> u8"Центрировать по группе", ImVec2(-1, 0)))
            {
                ParentTools->CenterToGroup();
            }
            if (ImGui::Button("Align To Object..."_RU >> u8"Выровнять по объекту...", ImVec2(-1, 0)))
            {
                ParentTools->AlignToObject();
            }
        }
        ImGui::Separator();
        ImGui::EndGroup();
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
}

void UIGroupTool::OnDrawUI()
{
    if (m_ChooseGroup)
    {
        xr_string in;
        bool      resutl;
        if (UIChooseForm::GetResult(resutl, in))
        {
            if (resutl)
            {
                m_Current = in;
                ParentTools->SetCurrentObject(m_Current.c_str());
            }
            m_ChooseGroup = false;
        }
        UIChooseForm::Update();
    }
}

void UIGroupTool::MultiSelByRefObject(bool clear_prev)
{
    ObjectList objlist;
    LPU32Vec   sellist;
    if (Scene->GetQueryObjects(objlist, OBJCLASS_GROUP, 1, 1, -1))
    {
        for (ObjectIt it = objlist.begin(); it != objlist.end(); it++)
        {
            LPCSTR   N  = ((CGroupObject*)*it)->RefName();
            ObjectIt _F = Scene->FirstObj(OBJCLASS_GROUP);
            ObjectIt _E = Scene->LastObj(OBJCLASS_GROUP);
            for (; _F != _E; _F++)
            {
                CGroupObject* _O = (CGroupObject*)(*_F);
                if ((*_F)->Visible() && _O->RefCompare(N))
                {
                    if (clear_prev)
                    {
                        _O->Select(false);
                        sellist.push_back((u32*)_O);
                    }
                    else
                    {
                        if (!_O->Selected())
                            sellist.push_back((u32*)_O);
                    }
                }
            }
        }
        std::sort(sellist.begin(), sellist.end());
        sellist.erase(std::unique(sellist.begin(), sellist.end()), sellist.end());

        std::mt19937 rnd{std::random_device()()};
        std::shuffle(sellist.begin(), sellist.end(), rnd);
        int max_k = iFloor(float(sellist.size()) / 100.f * float(m_selPercent) + 0.5f);
        int k     = 0;
        for (LPU32It o_it = sellist.begin(); k < max_k; o_it++, k++)
        {
            CGroupObject* _O = (CGroupObject*)(*o_it);
            _O->Select(true);
        }
    }
}

void UIGroupTool::SelByRefObject(bool flag)
{
    ObjectList objlist;

    if (m_Current.empty())
    {
        LPCSTR   N  = m_Current.c_str();
        ObjectIt _F = Scene->FirstObj(OBJCLASS_GROUP);
        ObjectIt _E = Scene->LastObj(OBJCLASS_GROUP);
        for (; _F != _E; _F++)
        {
            if ((*_F)->Visible())
            {
                CGroupObject* _O = (CGroupObject*)(*_F);
                if (_O->RefCompare(N))
                    _O->Select(flag);
            }
        }
    }
}
