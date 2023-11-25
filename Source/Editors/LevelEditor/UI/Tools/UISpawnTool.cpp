#include "stdafx.h"

#include <random>

UISpawnTool::UISpawnTool()
{
    m_selPercent = 100;
    m_Current    = nullptr;
    m_SpawnList  = xr_new<UIItemListForm>();
    m_SpawnList->SetOnItemFocusedEvent(TOnILItemFocused(this, &UISpawnTool::OnItemFocused));
    RefreshList();
    m_AttachObject = false;
}

UISpawnTool::~UISpawnTool()
{
    xr_delete(m_SpawnList);
}

void UISpawnTool::Draw()
{
    // ------------------------------------------------------------------------------------------------------ //
    if (ImGui::CollapsingHeader(("  Reference Select"_RU >> u8"  Ссылка выбранного"), ImGuiTreeNodeFlags_FramePadding | ImGuiTableFlags_NoBordersInBody))
    {
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SameLine(0, 10);
        ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "");
        ImGui::BeginGroup();
        {
            ImGui::Text("Select by Current:"_RU >> u8"Выбрать по текущему:   ");
            ImGui::SameLine(0, 12);
            if (ImGui::Button(" + "))
            {
                SelByRefObject(true);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SameLine(0, 12);
            if (ImGui::Button(" - "))
            {
                SelByRefObject(false);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::Text("Select by Selected:"_RU >> u8"Выбрать по выбранному:");
            ImGui::SameLine(0, 3);
            if (ImGui::Button("=%"))
            {
                MultiSelByRefObject(true);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SameLine(0, 3);
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
    if (ImGui::CollapsingHeader(("  Commands"_RU >> u8"  Управление"), ImGuiTreeNodeFlags_FramePadding | ImGuiTableFlags_NoBordersInBody))
    {
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SameLine(0, 10);
        ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "");
        ImGui::BeginGroup();
        {
            float size = float(ImGui::CalcItemWidth());
            {
                if (ImGui::Checkbox("Attach Object..."_RU >> u8"Присоединить объект...", &m_AttachObject))
                {
                    if (m_AttachObject)
                        ExecCommand(COMMAND_CHANGE_ACTION, etaAdd);
                }
                ImGui::SameLine(0, 15);
                if (ImGui::Button("Detach Object"_RU >> u8"Отсоединить объект", ImVec2(-1, 0)))
                {
                    ObjectList lst;
                    if (Scene->GetQueryObjects(lst, OBJCLASS_SPAWNPOINT, 1, 1, 0))
                    {
                        for (ObjectIt it = lst.begin(); it != lst.end(); it++)
                        {
                            CSpawnPoint* O = dynamic_cast<CSpawnPoint*>(*it);
                            R_ASSERT(O);
                            O->DetachObject();
                        }
                    }
                }
            }
        }
        ImGui::Separator();
        ImGui::EndGroup();
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    // ------------------------------------------------------------------------------------------------------ //
    ImGui::Separator();
    if (ImGui::CollapsingHeader(("  Object List"_RU >> u8"  Список Объектов"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTableFlags_NoBordersInBody))
    {
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SameLine(0, 10);
        ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "");
        ImGui::BeginChild("Object List"_RU >> u8"Список Объектов", ImVec2(0, 0), true, ImGuiStyleVar_WindowTitleAlign);
        ImGui::BeginGroup();
        m_SpawnList->Draw();
        ImGui::Separator();
        ImGui::EndGroup();
        ImGui::EndChild();
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
}

void UISpawnTool::SelByRefObject(bool flag)
{
    ObjectList objlist;
    LPCSTR     N = Current();
    if (N)
    {
        ObjectIt _F = Scene->FirstObj(OBJCLASS_SPAWNPOINT);
        ObjectIt _E = Scene->LastObj(OBJCLASS_SPAWNPOINT);
        for (; _F != _E; _F++)
        {
            if ((*_F)->Visible())
            {
                CSpawnPoint* _O = (CSpawnPoint*)(*_F);
                if (_O->RefCompare(N))
                    _O->Select(flag);
            }
        }
    }
}
void UISpawnTool::MultiSelByRefObject(bool clear_prev)
{
    ObjectList objlist;
    LPU32Vec   sellist;
    if (Scene->GetQueryObjects(objlist, OBJCLASS_SPAWNPOINT, 1, 1, -1))
    {
        for (ObjectIt it = objlist.begin(); it != objlist.end(); it++)
        {
            LPCSTR   N  = ((CSpawnPoint*)*it)->RefName();
            ObjectIt _F = Scene->FirstObj(OBJCLASS_SPAWNPOINT);
            ObjectIt _E = Scene->LastObj(OBJCLASS_SPAWNPOINT);
            for (; _F != _E; _F++)
            {
                CSpawnPoint* _O = (CSpawnPoint*)(*_F);
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
  	  	std::mt19937 rnd {std::random_device()()};
        std::shuffle(sellist.begin(), sellist.end(), rnd);
        int max_k = iFloor(float(sellist.size()) / 100.f * float(m_selPercent) + 0.5f);
        int k     = 0;
        for (LPU32It o_it = sellist.begin(); k < max_k; o_it++, k++)
        {
            CSpawnPoint* _O = (CSpawnPoint*)(*o_it);
            _O->Select(true);
        }
    }
}

void UISpawnTool::RefreshList()
{
    ListItemsVec items;
    LHelper().CreateItem(items, RPOINT_CHOOSE_NAME, 0, 0, RPOINT_CHOOSE_NAME);
    LHelper().CreateItem(items, ENVMOD_CHOOSE_NAME, 0, 0, ENVMOD_CHOOSE_NAME);
    CInifile::Root& data = ((CInifile*)pSettings)->sections();
    for (CInifile::RootIt it = data.begin(); it != data.end(); it++)
    {
        LPCSTR val;
        if ((*it)->line_exist("$spawn", &val))
        {
            shared_str caption = pSettings->r_string_wb((*it)->Name, "$spawn");
            shared_str sect    = (*it)->Name;
            if (caption.size())
            {
                ListItem* I = LHelper().CreateItem(items, caption.c_str(), 0, ListItem::flDrawThumbnail, (LPVOID) * (*it)->Name);
                // m_caption_to_sect[caption] = sect;
            }
        }
    }
    m_SpawnList->AssignItems(items);
}

void UISpawnTool::OnItemFocused(ListItem* item)
{
    m_Current = 0;
    if (item)
    {
        m_Current = (LPCSTR)item->m_Object;

        if (strcmp(item->Key(), RPOINT_CHOOSE_NAME) == 0)
            m_Current = RPOINT_CHOOSE_NAME;

        if (strcmp(item->Key(), ENVMOD_CHOOSE_NAME) == 0)
            m_Current = ENVMOD_CHOOSE_NAME;
    }
    ExecCommand(COMMAND_RENDER_FOCUS);
}
