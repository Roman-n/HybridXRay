#include "stdafx.h"

UIMainMenuForm::UIMainMenuForm() {}

UIMainMenuForm::~UIMainMenuForm() {}

void UIMainMenuForm::Draw()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"_RU >> u8"Файл"))
        {
            if (ImGui::MenuItem("Save"_RU >> u8"Сохранить"))
            {
                ExecCommand(COMMAND_SAVE);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::MenuItem("Reload"_RU >> u8"Перезагрузить"))
            {
                ExecCommand(COMMAND_LOAD);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::Separator();
            if (ImGui::MenuItem("Save .xr"_RU >> u8"Сохранить .xr"))
            {
                ExecCommand(COMMAND_SAVE_XR);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::MenuItem("Load .xr"_RU >> u8"Загрузить .xr"))
            {
                ExecCommand(COMMAND_LOAD_XR);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::Separator();
            if (ImGui::MenuItem("Validate"_RU >> u8"Валидация"))
            {
                ExecCommand(COMMAND_VALIDATE);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::MenuItem("Compact"_RU >> u8"Сжать"))
            {
                ExecCommand(COMMAND_COMPACT_PARTICLES);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::EndMenu();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SameLine();
        ImGui::Separator();
        ImGui::Separator();
        ImGui::SameLine();
        if (ImGui::BeginMenu("Editors"_RU >> u8"Эдиторы"))
        {
            if (ImGui::BeginMenu("Image"_RU >> u8"Текстуры"))
            {
                if (ImGui::MenuItem("Image Editor"_RU >> u8"Редактор Текстур", ""))
                {
                    ExecCommand(COMMAND_IMAGE_EDITOR);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::Separator();
                if (ImGui::MenuItem("Synchronize Textures"_RU >> u8"Синхронизировать Текстуры", ""))
                {
                    ExecCommand(COMMAND_REFRESH_TEXTURES);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                if (ImGui::MenuItem("Check New Textures"_RU >> u8"Поиск новой Текстуры", ""))
                {
                    ExecCommand(COMMAND_CHECK_TEXTURES);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::EndMenu();
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::BeginMenu("Sounds"_RU >> u8"Звуки"))
            {
                if (ImGui::MenuItem("Sound Editor"_RU >> u8"Редактор Звука", ""))
                {
                    ExecCommand(COMMAND_SOUND_EDITOR);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::Separator();
                if (ImGui::MenuItem("Synchronize Sounds"_RU >> u8"Синхронизировать Звуки", ""))
                {
                    ExecCommand(COMMAND_SYNC_SOUNDS);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::EndMenu();
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::MenuItem("Light Anim Editor"_RU >> u8"Редактор цветоанимаций(anm)", ""))
            {
                ExecCommand(COMMAND_LIGHTANIM_EDITOR);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::MenuItem("Minimap Editor"_RU >> u8"Редактор миникарты", ""))
            {
                ExecCommand(COMMAND_MINIMAP_EDITOR);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::EndMenu();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SameLine();
        ImGui::Separator();
        ImGui::Separator();
        ImGui::SameLine();
        if (ImGui::BeginMenu("Options"_RU >> u8"Опции"))
        {
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
                    bool selected[3] = {EDevice->dwFillMode == D3DFILL_POINT, EDevice->dwFillMode == D3DFILL_WIREFRAME, EDevice->dwFillMode == D3DFILL_SOLID};
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
                if (ImGui::BeginMenu("Shader Mode"_RU >> u8"Режим Теней"))
                {
                    bool selected[2] = {EDevice->dwShadeMode == D3DSHADE_FLAT, EDevice->dwShadeMode == D3DSHADE_GOURAUD};
                    if (ImGui::MenuItem("Flat"_RU >> u8"Плоские", "", &selected[0]))
                    {
                        EDevice->dwShadeMode = D3DSHADE_FLAT;
                        UI->RedrawScene();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    if (ImGui::MenuItem("Gouraud"_RU >> u8"Объёмные", "", &selected[1]))
                    {
                        EDevice->dwShadeMode = D3DSHADE_GOURAUD;
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
                    if (ImGui::MenuItem("Edged Faces"_RU >> u8"Показать рёбра", "", &selected))
                    {
                        psDeviceFlags.set(rsEdgedFaces, selected);
                        UI->RedrawScene();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                ImGui::Separator();
                {
                    bool selected = !HW.Caps.bForceGPU_SW;
                    if (ImGui::MenuItem("RenderHW"_RU >> u8"HW Рендер", "", &selected))
                    {
                        HW.Caps.bForceGPU_SW = !selected;
                        UI->Resize();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                ImGui::Separator();
                {
                    bool selected = psDeviceFlags.test(rsFilterLinear);
                    if (ImGui::MenuItem("Filter Linear"_RU >> u8"Линейный фильтр", "", &selected))
                    {
                        psDeviceFlags.set(rsFilterLinear, selected);
                        UI->RedrawScene();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                {
                    bool selected = psDeviceFlags.test(rsRenderTextures);
                    if (ImGui::MenuItem("Textures"_RU >> u8"Затекстурено", "", &selected))
                    {
                        psDeviceFlags.set(rsRenderTextures, selected);
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
            ImGui::Separator();
            {
                bool selected = psDeviceFlags.test(rsLighting);
                ;
                if (ImGui::MenuItem("Lighting"_RU >> u8"Выключить Свет", "", &selected))
                {
                    psDeviceFlags.set(rsLighting, selected);
                    UI->RedrawScene();
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            }
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
            ImGui::Separator();
            if (ImGui::MenuItem("Preferences"_RU >> u8"Настройки", ""))
            {
                ExecCommand(COMMAND_EDITOR_PREF);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::EndMenu();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SameLine();
        ImGui::Separator();
        ImGui::Separator();
        ImGui::SameLine();
        if (ImGui::BeginMenu("Windows"_RU >> u8"Окна"))
        {
            // Лог
            {
                bool selected = AllowLogCommands();

                if (ImGui::MenuItem("Log"_RU >> u8"Лог", "", &selected))
                {
                    ExecCommand(COMMAND_LOG_COMMANDS);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            }
            // Окно "Demo ImGui Helper"
            {
                if (ImGui::MenuItem("Demo ImGui Helper"_RU >> u8"Демонстрационный помощник ImGui", "", bDemoImGui))
                    bDemoImGui = !bDemoImGui;
                if (ImGui::IsItemHovered())
                {
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    ImGui::SetTooltip("Opens the ImGui Demo Helper, where you can get acquainted with ImGui functionality, visually see demo examples, etc. and so on.."_RU >> u8"Открывает Демонстрационный Хелпер по ImGui, где можно ознакомиться с функционалом ImGui, наглядно посмотреть демонстрационные примеры и т.д. и т.п..");
                }
            }
            ImGui::SameLine();
            ImGui::Separator();
            ImGui::Separator();
            ImGui::SameLine();
            // About
            {
                if (ImGui::MenuItem("About Editor"_RU >> u8"О Редакторе", ""))
                {
                    ExecCommand(COMMAND_ABOUT_EDITOR);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            }
            ImGui::EndMenu();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SameLine();
        ImGui::Separator();
        ImGui::SameLine(0, 5);
        ImGui::Separator();
        ImGui::SameLine(0, 5);
        ImGui::Separator();
        ImGui::SameLine();
        if (ImGui::BeginMenu("Language: EN/RU"_RU >> u8"Язык: EN/RU"))
        {
            if (ImGui::MenuItem("EN", "", EditorLocalization == ELocalization::EN))
            {
                EditorLocalization = ELocalization::EN;
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

            if (ImGui::MenuItem("RU", "", EditorLocalization == ELocalization::RU))
            {
                EditorLocalization = ELocalization::RU;
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

            ImGui::EndMenu();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::EndMainMenuBar();
    }
}
