﻿#include "stdafx.h"
#include "..\XrECore\Editor\EditorChooseEvents.h"
#include "..\resources\splash.h"
#include "..\XrEUI\ImGuizmo.h"
#include "Editor\Utils\Gizmo\IM_Manipulator.h"

UIMainForm* MainForm = nullptr;
UIMainForm::UIMainForm()
{
    EnableReceiveCommands();
    if (!ExecCommand(COMMAND_INITIALIZE, (u32)0, (u32)0))
    {
        FlushLog();
        exit(-1);
    }
    ExecCommand(COMMAND_UPDATE_GRID);
    ExecCommand(COMMAND_RENDER_FOCUS);
    FillChooseEvents();
    m_TopBar          = xr_new<UITopBarForm>();
    m_Render          = xr_new<UIRenderForm>();
    m_MainMenu        = xr_new<UIMainMenuForm>();
    m_LeftBar         = xr_new<UILeftBarForm>();
    m_Properties      = xr_new<UILPropertiesFrom>();
    m_WorldProperties = xr_new<UIWorldPropertiesFrom>();
    m_Render->SetContextMenuEvent(TOnRenderContextMenu(this, &UIMainForm::DrawContextMenu));
    m_Render->SetToolBarEvent(TOnRenderToolBar(this, &UIMainForm::DrawRenderToolBar));
    if (dynamic_cast<CLevelPreferences*>(EPrefs)->OpenObjectList)
    {
        UIObjectList::Show();
    }
    if (!dynamic_cast<CLevelPreferences*>(EPrefs)->OpenProperties)
    {
        m_Properties->Close();
    }
    if (!dynamic_cast<CLevelPreferences*>(EPrefs)->OpenWorldProperties)
    {
        m_WorldProperties->Close();
    }

    // Action
    m_tMenu         = EDevice->Resources->_CreateTexture("ed\\bar\\menu");
    m_tSelect       = EDevice->Resources->_CreateTexture("ed\\bar\\select");
    m_tAdd          = EDevice->Resources->_CreateTexture("ed\\bar\\add");
    m_tMove         = EDevice->Resources->_CreateTexture("ed\\bar\\move");
    m_tScale        = EDevice->Resources->_CreateTexture("ed\\bar\\scale");
    m_tRotate       = EDevice->Resources->_CreateTexture("ed\\bar\\rotate");

    // Snap
    m_tGSnap        = EDevice->Resources->_CreateTexture("ed\\bar\\gsnap");
    m_tOSnap        = EDevice->Resources->_CreateTexture("ed\\bar\\osnap");
    m_tMoveToSnap   = EDevice->Resources->_CreateTexture("ed\\bar\\movetosnap");
    m_tNSnap        = EDevice->Resources->_CreateTexture("ed\\bar\\nsnap");
    m_tVSnap        = EDevice->Resources->_CreateTexture("ed\\bar\\vsnap");
    m_tASnap        = EDevice->Resources->_CreateTexture("ed\\bar\\asnap");
    m_tMSnap        = EDevice->Resources->_CreateTexture("ed\\bar\\msnap");
    m_tZoom         = EDevice->Resources->_CreateTexture("ed\\bar\\zoom");
    m_tZoomSel      = EDevice->Resources->_CreateTexture("ed\\bar\\zoomsel");

    // Axis
    m_tShowAxisMove = EDevice->Resources->_CreateTexture("ed\\bar\\AxisMove");
    m_tX            = EDevice->Resources->_CreateTexture("ed\\bar\\AxisX");
    m_tY            = EDevice->Resources->_CreateTexture("ed\\bar\\AxisY");
    m_tZ            = EDevice->Resources->_CreateTexture("ed\\bar\\AxisZ");
    m_tZX           = EDevice->Resources->_CreateTexture("ed\\bar\\AxisZX");

    m_tGrid         = EDevice->Resources->_CreateTexture("ed\\bar\\grid");
    m_tScaleGrid    = EDevice->Resources->_CreateTexture("ed\\bar\\scale_grid");
    m_tAngle        = EDevice->Resources->_CreateTexture("ed\\bar\\angle");

    m_tShowPivot    = EDevice->Resources->_CreateTexture("ed\\bar\\pivot");
    m_tCsLocal      = EDevice->Resources->_CreateTexture("ed\\bar\\cslocal");
    m_tNuScale      = EDevice->Resources->_CreateTexture("ed\\bar\\nuscale");

    // View
    m_tVFront       = EDevice->Resources->_CreateTexture("ed\\bar\\ViewFront");
    m_tVBack        = EDevice->Resources->_CreateTexture("ed\\bar\\ViewB");
    m_tVLeft        = EDevice->Resources->_CreateTexture("ed\\bar\\ViewLeft");
    m_tVRight       = EDevice->Resources->_CreateTexture("ed\\bar\\ViewRight");
    m_tVTop         = EDevice->Resources->_CreateTexture("ed\\bar\\ViewTop");
    m_tVBottom      = EDevice->Resources->_CreateTexture("ed\\bar\\ViewB");
    m_tVReset       = EDevice->Resources->_CreateTexture("ed\\bar\\ViewReset");

    // Camera
    m_tPlaneMove    = EDevice->Resources->_CreateTexture("ed\\bar\\PlaneMove");
    m_tArcBall      = EDevice->Resources->_CreateTexture("ed\\bar\\ArcBall");
    m_tFreeFly      = EDevice->Resources->_CreateTexture("ed\\bar\\FreeFly");
}

UIMainForm::~UIMainForm()
{
    dynamic_cast<CLevelPreferences*>(EPrefs)->OpenProperties      = !m_Properties->IsClosed();
    dynamic_cast<CLevelPreferences*>(EPrefs)->OpenWorldProperties = !m_WorldProperties->IsClosed();
    dynamic_cast<CLevelPreferences*>(EPrefs)->OpenObjectList      = UIObjectList::IsOpen();
    ClearChooseEvents();
    xr_delete(m_WorldProperties);
    xr_delete(m_Properties);
    xr_delete(m_LeftBar);
    xr_delete(m_MainMenu);
    xr_delete(m_Render);
    xr_delete(m_TopBar);

    // Action
    m_tMenu.destroy();
    m_tSelect.destroy();
    m_tAdd.destroy();
    m_tMove.destroy();
    m_tScale.destroy();
    m_tRotate.destroy();

    // Snap
    m_tGSnap.destroy();
    m_tOSnap.destroy();
    m_tMoveToSnap.destroy();
    m_tNSnap.destroy();
    m_tVSnap.destroy();
    m_tASnap.destroy();
    m_tMSnap.destroy();

    // Axis
    m_tShowAxisMove.destroy();
    m_tX.destroy();
    m_tY.destroy();
    m_tZ.destroy();
    m_tZX.destroy();

    m_tZoom.destroy();
    m_tZoomSel.destroy();
    m_tGrid.destroy();
    m_tScaleGrid.destroy();
    m_tAngle.destroy();

    m_tShowPivot.destroy();
    m_tCsLocal.destroy();
    m_tNuScale.destroy();

    // View
    m_tVFront.destroy();
    m_tVBack.destroy();
    m_tVLeft.destroy();
    m_tVRight.destroy();
    m_tVTop.destroy();
    m_tVBottom.destroy();
    m_tVReset.destroy();

    // Camera
    m_tPlaneMove.destroy();
    m_tArcBall.destroy();
    m_tFreeFly.destroy();

    ExecCommand(COMMAND_DESTROY, (u32)0, (u32)0);
}

void UIMainForm::Draw()
{
    m_MainMenu->Draw();
    m_TopBar->Draw();
    m_LeftBar->Draw();
    m_Properties->Draw();
    m_WorldProperties->Draw();
    if (bDemoImGui)
        ImGui::ShowDemoWindow(&bDemoImGui);
    m_Render->Draw();
    splash::hide();
}

bool UIMainForm::Frame()
{
    if (UI)
        return UI->Idle();
    return false;
}

void UIMainForm::DrawContextMenu()
{
    // ------------------------------------------------------------------------------------------------------ //
    if (ImGui::BeginMenu("Edit"_RU >> u8"Правка"))
    {
        if (ImGui::MenuItem("Duplicate"_RU >> u8"Дублировать"))
        {
            ExecCommand(COMMAND_DUPLICATE);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::Separator();
        if (ImGui::MenuItem("Copy"_RU >> u8"Копировать"))
        {
            ExecCommand(COMMAND_COPY);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::MenuItem("Paste"_RU >> u8"Вставить"))
        {
            ExecCommand(COMMAND_PASTE);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::Separator();
        if (ImGui::MenuItem("Cut"_RU >> u8"Вырезать"))
        {
            ExecCommand(COMMAND_CUT);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::Separator();
        if (ImGui::MenuItem("Delete"_RU >> u8"Удалить"))
        {
            ExecCommand(COMMAND_DELETE_SELECTION);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::EndMenu();
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    // ------------------------------------------------------------------------------------------------------ //
    if (ImGui::BeginMenu("Visiblity"_RU >> u8"Видимость"))
    {
        if (ImGui::MenuItem("Hide Selected"_RU >> u8"Скрыть выделенное"))
        {
            ExecCommand(COMMAND_HIDE_SEL, FALSE);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::MenuItem("Hide Unselected"_RU >> u8"Скрыть невыделенное"))
        {
            ExecCommand(COMMAND_HIDE_UNSEL);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::MenuItem("Hide All"_RU >> u8"Скрыть ВСЁ"))
        {
            ExecCommand(COMMAND_HIDE_ALL, FALSE);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::Separator();
        if (ImGui::MenuItem("Unhide All"_RU >> u8"Показать ВСЁ"))
        {
            ExecCommand(COMMAND_HIDE_ALL, TRUE);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::EndMenu();
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    ImGui::Separator();
    // ------------------------------------------------------------------------------------------------------ //
    if (ImGui::MenuItem("Properties"_RU >> u8"Свойства"))
    {
        ExecCommand(COMMAND_SHOW_PROPERTIES);
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
}

void UIMainForm::DrawRenderToolBar(ImVec2 Pos, ImVec2 Size)
{
    // --------------------------------------------------------------------------------------------
    // Меню
    {
        ImGui::BeginGroup();
        m_tMenu->Load();
        {
            if (ImGui::BeginPopupContextItem("MenuScene"))
            {
                {
                    bool selected = psDeviceFlags.test(rsDrawSafeRect);
                    if (ImGui::MenuItem("Draw Safe Rect"_RU >> u8"Сжать вьюпорт", "", &selected))
                    {
                        psDeviceFlags.set(rsDrawSafeRect, selected);
                        UI->RedrawScene();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                {
                    bool selected = psDeviceFlags.test(rsDrawGrid);
                    if (ImGui::MenuItem("Draw Grid"_RU >> u8"Показать сетку", "", &selected))
                    {
                        psDeviceFlags.set(rsDrawGrid, selected);
                        UI->RedrawScene();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                ImGui::Separator();
                {
                    bool selected = psDeviceFlags.test(rsFog);
                    if (ImGui::MenuItem("Fog"_RU >> u8"Туман", "", &selected))
                    {
                        psDeviceFlags.set(rsFog, selected);
                        UI->RedrawScene();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                {
                    if (ImGui::BeginMenu("Environment"_RU >> u8"Погода"))
                    {
                        bool selected = !psDeviceFlags.test(rsEnvironment);
                        if (ImGui::MenuItem("None", "", &selected))
                        {
                            psDeviceFlags.set(rsEnvironment, false);
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        ImGui::Separator();
                        for (auto& i : g_pGamePersistent->Environment().WeatherCycles)
                        {
                            selected = psDeviceFlags.test(rsEnvironment) && i.first == g_pGamePersistent->Environment().CurrentCycleName;
                            if (ImGui::MenuItem(i.first.c_str(), "", &selected))
                            {
                                psDeviceFlags.set(rsEnvironment, true);
                                g_pGamePersistent->Environment().SetWeather(i.first.c_str(), true);
                                UI->RedrawScene();
                            }
                            if (ImGui::IsItemHovered())
                                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        }
                        ImGui::EndMenu();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                ImGui::Separator();
                if (ImGui::BeginMenu("Render"_RU >> u8"Экран"))
                {
                    if (ImGui::BeginMenu("Quality"_RU >> u8"Качество"))
                    {
                        static bool selected[4] = {false, false, true, false};
                        if (ImGui::MenuItem("25%", "", &selected[0]))
                        {
                            selected[1] = selected[2] = selected[3] = false;
                            UI->SetRenderQuality(1 / 4.f);
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        if (ImGui::MenuItem("50%", "", &selected[1]))
                        {
                            selected[0] = selected[2] = selected[3] = false;
                            UI->SetRenderQuality(1 / 2.f);
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        if (ImGui::MenuItem("100%", "", &selected[2]))
                        {
                            selected[1] = selected[0] = selected[3] = false;
                            UI->SetRenderQuality(1.f);
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        if (ImGui::MenuItem("200%", "", &selected[3]))
                        {
                            selected[1] = selected[2] = selected[0] = false;
                            UI->SetRenderQuality(2.f);
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        ImGui::EndMenu();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    if (ImGui::BeginMenu("Fill Mode"_RU >> u8"Режим отображения"))
                    {
                        bool selected[3] =
                        {
                            EDevice->dwFillMode == D3DFILL_POINT,
                            EDevice->dwFillMode == D3DFILL_WIREFRAME,
                            EDevice->dwFillMode == D3DFILL_SOLID
                        };
                        if (ImGui::MenuItem("Point"_RU >> u8"Точки", "", &selected[0]))
                        {
                            EDevice->dwFillMode = D3DFILL_POINT;
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        if (ImGui::MenuItem("Wireframe"_RU >> u8"Рёбра", "", &selected[1]))
                        {
                            EDevice->dwFillMode = D3DFILL_WIREFRAME;
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        if (ImGui::MenuItem("Solid"_RU >> u8"Норма", "", &selected[2]))
                        {
                            EDevice->dwFillMode = D3DFILL_SOLID;
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        ImGui::EndMenu();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    {
                        bool selected = psDeviceFlags.test(rsEdgedFaces);
                        if (ImGui::MenuItem("Edged Faces"_RU >> u8"Показать Рёбра", "", &selected))
                        {
                            psDeviceFlags.set(rsEdgedFaces, selected);
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    }
                    {
                        bool selected = psDeviceFlags.test(rsLighting);
                        if (ImGui::MenuItem("Lighting"_RU >> u8"Выключить Свет", "", &selected))
                        {
                            psDeviceFlags.set(rsLighting, selected);
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::Separator();

                {
                    bool selected = psDeviceFlags.test(rsMuteSounds);
                    if (ImGui::MenuItem("Mute Sounds"_RU >> u8"Отключить звуки", "", &selected))
                    {
                        psDeviceFlags.set(rsMuteSounds, selected);
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                {
                    bool selected = psDeviceFlags.test(rsRenderRealTime);
                    if (ImGui::MenuItem("Real Time"_RU >> u8"В реальном времени", "", &selected))
                    {
                        psDeviceFlags.set(rsRenderRealTime, selected);
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                ImGui::Separator();
                {
                    bool selected = psDeviceFlags.test(rsStatistic);
                    if (ImGui::MenuItem("Stats"_RU >> u8"Статистика", "", &selected))
                    {
                        psDeviceFlags.set(rsStatistic, selected);
                        UI->RedrawScene();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                ImGui::EndPopup();
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::ImageButton(m_tMenu->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                ImGui::OpenPopup("MenuScene");
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Menu"_RU >> u8"Меню");
            }
        }
        ImGui::EndGroup();
    }
    ImGui::SameLine(0, ImGui::GetFontSize() * 1.5);
    // --------------------------------------------------------------------------------------------
    // Action
    {
        Fvector p, n;
        ETAction Action = LTools->GetAction();
        ImGui::BeginGroup();
        // --------------------------------------------------------------------------------------------
        // Select
        {
            bool bPushColor = false;
            if (Action == etaSelect)
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                if (LUI->ScenePickObjectGeometry(p, UI->m_CurrentRStart, UI->m_CurrentRDir, 1, &n))
                    ImGui::SetMouseCursor(ImGuiMouseCursor_СrossSelect);
            }
            m_tSelect->Load();
            if (ImGui::ImageButton(m_tSelect->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                LTools->SetAction(etaSelect);
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Select"_RU >> u8"Выбрать");
        }
        ImGui::SameLine();
        // --------------------------------------------------------------------------------------------
        // Add
        {
            bool bPushColor = false;
            if (Action == etaAdd)
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                if (LUI->ScenePickObjectGeometry(p, UI->m_CurrentRStart, UI->m_CurrentRDir, 1, &n))
                     ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
            }
            m_tAdd->Load();
            if (ImGui::ImageButton(m_tAdd->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                LTools->SetAction(etaAdd);
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Add"_RU >> u8"Установить в сцене");
        }
        ImGui::SameLine();
        // --------------------------------------------------------------------------------------------
        // Move
        {
            bool bPushColor = false;
            if (Action == etaMove)
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                if (LUI->ScenePickObjectGeometry(p, UI->m_CurrentRStart, UI->m_CurrentRDir, 1, &n))
                {
                    if (!EPrefs->tools_show_move_axis)
                        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
                    else
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
            }
            m_tMove->Load();
            if (ImGui::ImageButton(m_tMove->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                LTools->SetAction(etaMove);
            }

            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Move"_RU >> u8"Двигать");
        }
        ImGui::SameLine();
        // --------------------------------------------------------------------------------------------
        // Scale
        {
            bool bPushColor = false;
            if (Action == etaScale)
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                if (LUI->ScenePickObjectGeometry(p, UI->m_CurrentRStart, UI->m_CurrentRDir, 1, &n))
                    ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
            }
            m_tScale->Load();
            if (ImGui::ImageButton(m_tScale->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                LTools->SetAction(etaScale);
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Scale"_RU >> u8"Масштабирование");
        }
        ImGui::SameLine();
        // --------------------------------------------------------------------------------------------
        // Rotate
        {
            bool bPushColor = false;
            if (Action == etaRotate)
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                if (LUI->ScenePickObjectGeometry(p, UI->m_CurrentRStart, UI->m_CurrentRDir, 1, &n))
                    ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            }
            m_tRotate->Load();
            if (ImGui::ImageButton(m_tRotate->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                LTools->SetAction(etaRotate);
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Rotate"_RU >> u8"Вращать");
        }
        ImGui::EndGroup();
    }
    ImGui::SameLine(0, ImGui::GetFontSize() * 1.5);
    // --------------------------------------------------------------------------------------------
    // Привязка и фокус
    {
        ImGui::BeginGroup();
        // --------------------------------------------------------------------------------------------
        // Привязка к объектам
        {
            bool bPushColor = false;
            if (Tools->GetSettings(etfOSnap))
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tOSnap->Load();
            if (ImGui::ImageButton(m_tOSnap->surface_get(), ImVec2(16, ImGui::GetFontSize()), ImVec2(0, 0), ImVec2(0.5f, 1.f)))
            {
                ExecCommand(COMMAND_SET_SETTINGS, etfOSnap, !Tools->GetSettings(etfOSnap));
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Object Snap Toggle"_RU >> u8"Привязка к объектам");
        }
        ImGui::SameLine();
        // --------------------------------------------------------------------------------------------
        // Переключатель перемещения привязки к объекту
        {
            bool bPushColor = false;
            if (Tools->GetSettings(etfMTSnap))
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tMoveToSnap->Load();
            if (ImGui::ImageButton(m_tMoveToSnap->surface_get(), ImVec2(16, ImGui::GetFontSize()), ImVec2(0, 0), ImVec2(0.5f, 1.f)))
            {
                ExecCommand(COMMAND_SET_SETTINGS, etfMTSnap, !Tools->GetSettings(etfMTSnap));
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Moving Snap To Object Toggle"_RU >> u8"Переключатель перемещения привязки к объекту");
        }
        ImGui::SameLine();
        // --------------------------------------------------------------------------------------------
        // Привязка к Нормалям
        {
            bool bPushColor = false;
            if (Tools->GetSettings(etfNormalAlign))
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tNSnap->Load();
            if (ImGui::ImageButton(m_tNSnap->surface_get(), ImVec2(16, ImGui::GetFontSize()), ImVec2(0, 0), ImVec2(0.5f, 1.f)))
            {
                ExecCommand(COMMAND_SET_SETTINGS, etfNormalAlign, !Tools->GetSettings(etfNormalAlign));
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Normal Alignment"_RU >> u8"Привязка к Нормалям");
        }
        ImGui::SameLine(0, ImGui::GetFontSize() * 1);
        // --------------------------------------------------------------------------------------------
        // Фокусировка
        {
            // Оптимальный вид - вся сцена
            m_tZoom->Load();
            if (ImGui::ImageButton(m_tZoom->surface_get(), ImVec2(16, ImGui::GetFontSize()), ImVec2(0, 0), ImVec2(0.5f, 1.f)))
            {
                ExecCommand(COMMAND_ZOOM_EXTENTS, FALSE);
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Focus the whole scene"_RU >> u8"Оптимальный вид - вся сцена");
            }
            ImGui::SameLine();
            // --------------------------------------------------------------------------------------------
            // Сфокусироваться на выбранном объекте
            m_tZoomSel->Load();
            if (ImGui::ImageButton(m_tZoomSel->surface_get(), ImVec2(16, ImGui::GetFontSize()), ImVec2(0, 0), ImVec2(0.5f, 1.f)))
            {
                ExecCommand(COMMAND_ZOOM_EXTENTS, TRUE);
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Focus on the selected object"_RU >> u8"Сфокусироваться на выбранном объекте");
            }
        }
        ImGui::SameLine(0, ImGui::GetFontSize() * 1);
        // --------------------------------------------------------------------------------------------
        // Привязка|Snap
        {
            ImGui::BeginGroup();
            // Привязка к сетке
            {
                bool bPushColor = false;
                if (Tools->GetSettings(etfGSnap))
                {
                    bPushColor = true;
                    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                }
                m_tGSnap->Load();
                if (ImGui::ImageButton(m_tGSnap->surface_get(), ImVec2(16, ImGui::GetFontSize()), ImVec2(0, 0), ImVec2(0.5f, 1.f)))
                {
                    ExecCommand(COMMAND_SET_SETTINGS, etfGSnap, !Tools->GetSettings(etfGSnap));
                }
                if (bPushColor)
                {
                    ImGui::PopStyleColor();
                    ImGui::PopStyleColor();
                }
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Grid Snap Toggle"_RU >> u8"Привязка к сетке");
            }
            ImGui::SameLine();
            // --------------------------------------------------------------------------------------------
            // Привязка к вершинам
            {
                bool bPushColor = false;
                if (Tools->GetSettings(etfVSnap))
                {
                    bPushColor = true;
                    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                }
                m_tVSnap->Load();
                if (ImGui::ImageButton(m_tVSnap->surface_get(), ImVec2(16, ImGui::GetFontSize()), ImVec2(0, 0), ImVec2(0.5f, 1.f)))
                {
                    ExecCommand(COMMAND_SET_SETTINGS, etfVSnap, !Tools->GetSettings(etfVSnap));
                }
                if (bPushColor)
                {
                    ImGui::PopStyleColor();
                    ImGui::PopStyleColor();
                }
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Vertex Snap Toggle"_RU >> u8"Привязка к вершинам");
            }
            ImGui::SameLine();
            // --------------------------------------------------------------------------------------------
            // Привязка к углу
            {
                bool bPushColor = false;
                if (Tools->GetSettings(etfASnap))
                {
                    bPushColor = true;
                    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                }
                m_tASnap->Load();
                if (ImGui::ImageButton(m_tASnap->surface_get(), ImVec2(16, ImGui::GetFontSize()), ImVec2(0, 0), ImVec2(0.5f, 1.f)))
                {
                    ExecCommand(COMMAND_SET_SETTINGS, etfASnap, !Tools->GetSettings(etfASnap));
                }
                if (bPushColor)
                {
                    ImGui::PopStyleColor();
                    ImGui::PopStyleColor();
                }
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Angle Snap Toggle"_RU >> u8"Привязка к углу");
            }
            ImGui::SameLine();
            // --------------------------------------------------------------------------------------------
            // Привязка к перемещению
            {
                bool bPushColor = false;
                if (Tools->GetSettings(etfMSnap))
                {
                    bPushColor = true;
                    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                }
                m_tMSnap->Load();
                if (ImGui::ImageButton(m_tMSnap->surface_get(), ImVec2(16, ImGui::GetFontSize()), ImVec2(0, 0), ImVec2(0.5f, 1.f)))
                {
                    ExecCommand(COMMAND_SET_SETTINGS, etfMSnap, !Tools->GetSettings(etfMSnap));
                }
                if (bPushColor)
                {
                    ImGui::PopStyleColor();
                    ImGui::PopStyleColor();
                }
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Moving Snap Toggle"_RU >> u8"Привязка к перемещению");
            }
            ImGui::EndGroup();
        }
        ImGui::EndGroup();
    }
    /*/ --------------------------------------------------------------------------------------------
    {
        string_path Temp;
        ImGui::SameLine(0, ImGui::GetFontSize() * 2);
        ImGui::BeginGroup();

        {
            bool bPushColor = false;

            if (LTools->GetGimzo()->IsStepEnable(Gizmo::EType::Move))
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tGrid->Load();
            if (ImGui::ImageButton(m_tGrid->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                LTools->GetGimzo()->SwitchStep(Gizmo::EType::Move, !LTools->GetGimzo()->IsStepEnable(Gizmo::EType::Move));
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Fixed object movement"_RU >> u8"Фиксированное перемещение объекта");
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 4);
        xr_sprintf(Temp, "%.2f", LTools->GetGimzo()->GetStep(Gizmo::EType::Move));
        if (ImGui::BeginCombo("##move", Temp, ImGuiComboFlags_None))
        {
            if (ImGui::Selectable("0.01", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Move, 0.01f);
            }
            if (ImGui::Selectable("0.05", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Move, 0.05f);
            }
            if (ImGui::Selectable("0.1", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Move, 0.1f);
            }
            if (ImGui::Selectable("1", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Move, 1.f);
            }
            if (ImGui::Selectable("2", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Move, 2.f);
            }
            if (ImGui::Selectable("5", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Move, 5.f);
            }
            if (ImGui::Selectable("50", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Move, 50.f);
            }
            if (ImGui::Selectable("200", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Move, 200.f);
            }
            if (ImGui::Selectable("1000", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Move, 1000.f);
            }
            ImGui::EndCombo();
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("The choice of a fixed distance of movement of the object"_RU >> u8"Выбор фиксированного растояния перемещения объекта");
        }
        ImGui::SameLine(0, ImGui::GetFontSize());

        {
            bool bPushColor = false;
            if (LTools->GetGimzo()->IsStepEnable(Gizmo::EType::Scale))
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tScaleGrid->Load();
            if (ImGui::ImageButton(m_tScaleGrid->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                LTools->GetGimzo()->SwitchStep(Gizmo::EType::Scale, !LTools->GetGimzo()->IsStepEnable(Gizmo::EType::Scale));
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Fixed Object Scaling"_RU >> u8"Фиксированное масштабирование объкта");
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }

        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 4);
        xr_sprintf(Temp, "%.2f", LTools->GetGimzo()->GetStep(Gizmo::EType::Scale));
        if (ImGui::BeginCombo("##scale", Temp, ImGuiComboFlags_None))
        {
            if (ImGui::Selectable("0.01", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Scale, 0.01f);
            }
            if (ImGui::Selectable("0.05", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Scale, 0.05f);
            }
            if (ImGui::Selectable("0.1", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Scale, 0.1f);
            }
            if (ImGui::Selectable("1", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Scale, 1.f);
            }
            if (ImGui::Selectable("2", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Scale, 2.f);
            }
            if (ImGui::Selectable("5", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Scale, 5.f);
            }
            if (ImGui::Selectable("50", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Scale, 50.f);
            }
            if (ImGui::Selectable("200", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Scale, 200.f);
            }
            if (ImGui::Selectable("1000", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Scale, 1000.f);
            }
            ImGui::EndCombo();
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Setting a Fixed Object Scaling"_RU >> u8"Установка фиксированного масштабирования объкта");
        }
        ImGui::SameLine(0, ImGui::GetFontSize());

        {
            bool bPushColor = false;
            if (LTools->GetGimzo()->IsStepEnable(Gizmo::EType::Rotate))
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tAngle->Load();
            if (ImGui::ImageButton(m_tAngle->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                LTools->GetGimzo()->SwitchStep(Gizmo::EType::Rotate, !LTools->GetGimzo()->IsStepEnable(Gizmo::EType::Rotate));
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Fixed object rotation angle"_RU >> u8"Фиксированный угол вращения объкта");
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }

        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
        xr_sprintf(Temp, "%.5f", LTools->GetGimzo()->GetStep(Gizmo::EType::Rotate));
        if (ImGui::BeginCombo("##rotate", Temp, ImGuiComboFlags_None))
        {
            if (ImGui::Selectable("1.40625", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Rotate, 1.40625f);
            }
            if (ImGui::Selectable("2.8125", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Rotate, 2.8125f);
            }
            if (ImGui::Selectable("5.625", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Rotate, 5.625f);
            }
            if (ImGui::Selectable("11.25", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Rotate, 11.25f);
            }
            if (ImGui::Selectable("22.5", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Rotate, 22.5f);
            }
            if (ImGui::Selectable("45", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Rotate, 45.f);
            }
            if (ImGui::Selectable("90", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Rotate, 90.f);
            }
            if (ImGui::Selectable("180", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Rotate, 180.f);
            }
            ImGui::EndCombo();
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Set a fixed rotation angle of the object (in degrees)"_RU >> u8"Установка фиксированного угла вращения объкта(в градусах)");
        }
        ImGui::EndGroup();
    }
    // --------------------------------------------------------------------------------------------*/
    ImGui::NewLine();
    // --------------------------------------------------------------------------------------------
    // Pivot ...
    {
        ImGui::BeginGroup();
        // --------------------------------------------------------------------------------------------
        // Показать Pivot выбранного объекта
        {
            bool bPushColor = false;
            if (EPrefs->object_flags.is(epoDrawPivot))
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tShowPivot->Load();
            if (ImGui::ImageButton(m_tShowPivot->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                if (!EPrefs->object_flags.is(epoDrawPivot))
                    EPrefs->object_flags.set(epoDrawPivot, true);
                else
                    EPrefs->object_flags.set(epoDrawPivot, false);
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Show Pivot of selected object"_RU >> u8"Показать Pivot выбранного объекта");
        }
        ImGui::Spacing();
        // --------------------------------------------------------------------------------------------
        // Parent CS Toggle
        {
            bool bPushColor = false;
            if (Tools->GetSettings(etfCSParent))
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tCsLocal->Load();
            if (ImGui::ImageButton(m_tCsLocal->surface_get(), ImVec2(16, ImGui::GetFontSize()), ImVec2(0, 0), ImVec2(0.5f, 1.f)))
            {
                ExecCommand(COMMAND_SET_SETTINGS, etfCSParent, !Tools->GetSettings(etfCSParent));
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Parent CS Toggle"_RU >> u8"Parent CS Toggle");
        }
        ImGui::Spacing();
        // --------------------------------------------------------------------------------------------
        // Маштабирование по осям
        {
            bool bPushColor = false;
            if (Tools->GetSettings(etfNUScale))
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tNuScale->Load();
            if (ImGui::ImageButton(m_tNuScale->surface_get(), ImVec2(16, ImGui::GetFontSize()), ImVec2(0, 0), ImVec2(0.5f, 1.f)))
            {
                ExecCommand(COMMAND_SET_SETTINGS, etfNUScale, !Tools->GetSettings(etfNUScale));
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Scaling by Axes only"_RU >> u8"Маштабирование только по Осям");
        }
        ImGui::EndGroup();
    }
    // --------------------------------------------------------------------------------------------
    ImGui::NewLine();
    // --------------------------------------------------------------------------------------------
    // Выбор Осей.
    {
        ImGui::BeginGroup();
        // --------------------------------------------------------------------------------------------
        // Show Move Axis
        {
            bool bPushColor = false;
            if (EPrefs->tools_show_move_axis)
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tShowAxisMove->Load();
            if (ImGui::ImageButton(m_tShowAxisMove->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                if (EPrefs->tools_show_move_axis)
                {
                    EPrefs->tools_show_move_axis = false;
                }
                else
                {
                    EPrefs->tools_show_move_axis = true;
                    LTools->SetAction(etaMove);
                }
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Show Move Axis"_RU >> u8"Показать Ось Перемещения");
        }
        ImGui::Spacing();
        // --------------------------------------------------------------------------------------------
        ETAxis Axis = LTools->GetAxis();
        // --------------------------------------------------------------------------------------------
        // Ось X
        {
            bool bPushColor = false;
            if (Axis == etAxisX)
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tX->Load();
            if (ImGui::ImageButton(m_tX->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                ExecCommand(COMMAND_CHANGE_AXIS, etAxisX, !LTools->GetSettings(etAxisX));
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Select X Axis"_RU >> u8"Выбрать Ось X");
        }
        ImGui::Spacing();
        // --------------------------------------------------------------------------------------------
        // Ось Y
        {
            bool bPushColor = false;
            if (Axis == etAxisY)
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tY->Load();
            if (ImGui::ImageButton(m_tY->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                ExecCommand(COMMAND_CHANGE_AXIS, etAxisY, !LTools->GetSettings(etAxisY));
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Select Y Axis"_RU >> u8"Выбрать Ось Y");
        }
        ImGui::Spacing();
        // --------------------------------------------------------------------------------------------
        // Ось Z
        {
            bool bPushColor = false;
            if (Axis == etAxisZ)
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tZ->Load();
            if (ImGui::ImageButton(m_tZ->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                ExecCommand(COMMAND_CHANGE_AXIS, etAxisZ, !LTools->GetSettings(etAxisZ));
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Select Z Axis"_RU >> u8"Выбрать Ось Z");
        }
        ImGui::Spacing();
        // --------------------------------------------------------------------------------------------
        // Ось ZX
        {
            bool bPushColor = false;
            if (Axis == etAxisZX)
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tZX->Load();
            if (ImGui::ImageButton(m_tZX->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                ExecCommand(COMMAND_CHANGE_AXIS, etAxisZX, !LTools->GetSettings(etAxisZX));
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Select ZX Axis"_RU >> u8"Выбрать Ось ZX");
        }
        ImGui::EndGroup();
    }
    // --------------------------------------------------------------------------------------------
    ImGui::NewLine();
    // --------------------------------------------------------------------------------------------
    // View
    {
        ImGui::BeginGroup();
        // --------------------------------------------------------------------------------------------
        // Вид спереди.
        {
            m_tVFront->Load();
            {
                if (ImGui::ImageButton(m_tVFront->surface_get(), ImVec2(16, ImGui::GetFontSize())))
                {
                    EDevice->m_Camera.ViewFront();
                    UI->RedrawScene();
                }
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Front View"_RU >> u8"Вид спереди");
            }
        }
        ImGui::Spacing();
        // --------------------------------------------------------------------------------------------
        // Вид сзади.
        {
            m_tVBack->Load();
            {
                if (ImGui::ImageButton(m_tVBack->surface_get(), ImVec2(16, ImGui::GetFontSize())))
                {
                    EDevice->m_Camera.ViewBack();
                    UI->RedrawScene();
                }
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Back View"_RU >> u8"Вид сзади");
            }
        }
        ImGui::Spacing();
        // --------------------------------------------------------------------------------------------
        // Вид слева.
        {
            m_tVLeft->Load();
            {
                if (ImGui::ImageButton(m_tVLeft->surface_get(), ImVec2(16, ImGui::GetFontSize())))
                {
                    EDevice->m_Camera.ViewLeft();
                    UI->RedrawScene();
                }
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Left View"_RU >> u8"Вид слева");
            }
        }
        ImGui::Spacing();
        // --------------------------------------------------------------------------------------------
        // Вид справа.
        {
            m_tVRight->Load();
            {
                if (ImGui::ImageButton(m_tVRight->surface_get(), ImVec2(16, ImGui::GetFontSize())))
                {
                    EDevice->m_Camera.ViewRight();
                    UI->RedrawScene();
                }
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Right View"_RU >> u8"Вид справа");
            }
        }
        ImGui::Spacing();
        // --------------------------------------------------------------------------------------------
        // Вид сверху.
        {
            m_tVTop->Load();
            {
                if (ImGui::ImageButton(m_tVTop->surface_get(), ImVec2(16, ImGui::GetFontSize())))
                {
                    EDevice->m_Camera.ViewTop();
                    UI->RedrawScene();
                }
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Top View"_RU >> u8"Вид сверху");
            }
        }
        ImGui::Spacing();
        // --------------------------------------------------------------------------------------------
        // Вид снизу.
        {
            m_tVBottom->Load();
            {
                if (ImGui::ImageButton(m_tVBottom->surface_get(), ImVec2(16, ImGui::GetFontSize())))
                {
                    EDevice->m_Camera.ViewBottom();
                    UI->RedrawScene();
                }
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Bottom View"_RU >> u8"Вид снизу");
            }
        }
        ImGui::Spacing();
        // --------------------------------------------------------------------------------------------
        // Сбросить Вид.
        {
            m_tVReset->Load();
            {
                if (ImGui::ImageButton(m_tVReset->surface_get(), ImVec2(16, ImGui::GetFontSize())))
                {
                    EDevice->m_Camera.ViewReset();
                    UI->RedrawScene();
                }
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Reset View"_RU >> u8"Сбросить Вид");
            }
        }
        ImGui::EndGroup();
    }
    // --------------------------------------------------------------------------------------------
    ImGui::NewLine();
    // --------------------------------------------------------------------------------------------
    // Camera
    {
        ImGui::BeginGroup();
        ECameraStyle Camera = EDevice->m_Camera.GetStyle();
        // --------------------------------------------------------------------------------------------
        // Свободный режим камеры
        {
            bool bPushColor = false;
            if (Camera == csPlaneMove)
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tPlaneMove->Load();
            if (ImGui::ImageButton(m_tPlaneMove->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                EDevice->m_Camera.SetStyle(csPlaneMove);
                UI->RedrawScene();
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Free camera mode"_RU >> u8"Свободный режим камеры");
        }
        ImGui::Spacing();
        // --------------------------------------------------------------------------------------------
        // Привязка камеры к центру координат|сцены
        {
            bool bPushColor = false;
            if (Camera == cs3DArcBall)
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tArcBall->Load();
            if (ImGui::ImageButton(m_tArcBall->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                EDevice->m_Camera.SetStyle(cs3DArcBall);
                UI->RedrawScene();
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Snap the camera to the center of coordinates|scene"_RU >> u8"Привязка камеры к центру координат|сцены");
        }
        ImGui::Spacing();
        // --------------------------------------------------------------------------------------------
        // Автооблёт сцены камерой
        {
            bool bPushColor = false;
            if (Camera == csFreeFly)
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tFreeFly->Load();
            if (ImGui::ImageButton(m_tFreeFly->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                EDevice->m_Camera.SetStyle(csFreeFly);
                UI->RedrawScene();
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Automatic camera flyover of the scene"_RU >> u8"Автооблёт сцены камерой");
        }
        ImGui::EndGroup();
    }
    // --------------------------------------------------------------------------------------------
    // Gizmo
    {
      imManipulator.Render(Pos.x, Pos.y, Size.x, Size.y);
    }


    


}
