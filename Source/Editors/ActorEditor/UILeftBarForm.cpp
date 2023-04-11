﻿#include "stdafx.h"

UILeftBarForm::UILeftBarForm()
{
    m_RenderMode = Render_Editor;
    m_PickMode   = 0;
}

UILeftBarForm::~UILeftBarForm() {}

void UILeftBarForm::Draw()
{
    ImGui::Begin("Tool Bar"_RU >> u8"Панель инструментов", 0);
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

    // ------------------------------------------------------------------------------------------------------ //
    ImGui::BeginChild((ImVec4(0.75, 1.5, 0, 0.85), "Model"_RU >> u8"Объект"), ImVec2(0, 100), true, ImGuiStyleVar_WindowTitleAlign);
    {
        ImGui::SameLine(0, 10);
        ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "   Model"_RU >> u8"   Объект", ImGuiStyleVar_WindowTitleAlign);
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 2));
        ImGui::AlignTextToFramePadding();
        if (ImGui::Button("Bone Part"_RU >> u8"Части костей"))
        {
            UIBoneForm::Show();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Render Style:"_RU >> u8"Стиль рендера:");
        ImGui::SameLine();
        if (ImGui::RadioButton("Editor"_RU >> u8"Эдитор", m_RenderMode == Render_Editor))
        {
            ATools->PhysicsStopSimulate();
            m_RenderMode = Render_Editor;
            ExecCommand(COMMAND_UPDATE_PROPERTIES);
            UI->RedrawScene();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SameLine();
        if (ImGui::RadioButton("Engine"_RU >> u8"Движок", m_RenderMode == Render_Engine))
        {
            ATools->PhysicsStopSimulate();
            m_RenderMode = Render_Engine;
            if (!ATools->IsVisualPresent())
                ExecCommand(COMMAND_MAKE_PREVIEW);
            if (!ATools->IsVisualPresent())
                SetRenderMode(false);
            else
                SetRenderMode(true);
            ExecCommand(COMMAND_UPDATE_PROPERTIES);
            UI->RedrawScene();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SameLine(0, 10);
        if (ImGui::Button("Clip Maker"))
        {
            // UIBoneForm::Show();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        static const char* PickModeList[] = {"None", "Surface", "Bone", "Object", "Motios"};
        ImGui::Combo("Pick mode"_RU >> u8"Выбор режима", &m_PickMode, PickModeList, 5, -1);
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::EndChild();
    }
    ImGui::Separator();
    // ------------------------------------------------------------------------------------------------------ //
    ImGui::BeginChild("Object Items"_RU >> u8"Структура объекта", ImVec2(0, 233), true, ImGuiStyleVar_WindowTitleAlign);
    {
        ImGui::SameLine(0, 10);
        ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "   Object Items"_RU >> u8"   Структура объекта", ImGuiStyleVar_WindowTitleAlign);
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 2));
        ImGui::AlignTextToFramePadding();
        ImGui::BeginGroup();
        ATools->m_ObjectItems->Draw();
        ImGui::EndGroup();
        ImGui::EndChild();
    }
    ImGui::Separator();
    // ------------------------------------------------------------------------------------------------------ //
    if (ImGui::BeginChild("Item Properties"_RU >> u8"Свойства объекта", ImVec2(0, 0), true, ImGuiStyleVar_WindowTitleAlign));
    {
        ImGui::SameLine(0, 10);
        ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "   Item Properties"_RU >> u8"   Свойства объекта", ImGuiStyleVar_WindowTitleAlign);
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 2));
        ImGui::AlignTextToFramePadding();
        ImGui::BeginGroup();
        ATools->m_Props->Draw();
        ImGui::EndGroup();
        ImGui::EndChild();
    }
    ImGui::End();
}

void UILeftBarForm::SetRenderMode(bool bEngineMode)
{
    if (ATools->IsVisualPresent() && bEngineMode)
        m_RenderMode = Render_Engine;
    else
        m_RenderMode = Render_Editor;
    ATools->PlayMotion();
}
