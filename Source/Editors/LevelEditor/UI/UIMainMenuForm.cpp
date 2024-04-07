#include "stdafx.h"
UIMainMenuForm::UIMainMenuForm() {}

UIMainMenuForm::~UIMainMenuForm() {}

void UIMainMenuForm::Draw()
{
    if (ImGui::BeginMainMenuBar())
    {
        // --------------------------------------------------------------------------------------------
        // Файл
        if (ImGui::BeginMenu("File"_RU >> u8"Файл"))
        {
            if (ImGui::MenuItem("Clear"_RU >> u8"Очистить", ""))
            {
                ExecCommand(COMMAND_CLEAR);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::MenuItem("Open..."_RU >> u8"Открыть...", ""))
            {
                ExecCommand(COMMAND_LOAD);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::MenuItem("Save"_RU >> u8"Сохранить", ""))
            {
                ExecCommand(COMMAND_SAVE, xr_string(LTools->m_LastFileName.c_str()));
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::MenuItem("Save As ..."_RU >> u8"Сохранить как ...", ""))
            {
                ExecCommand(COMMAND_SAVE, 0, 1);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::Separator();
            if (ImGui::MenuItem("Open Selection..."_RU >> u8"Открыть Выделенное...", ""))
            {
                ExecCommand(COMMAND_LOAD_SELECTION);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::MenuItem("Save Selection As..."_RU >> u8"Сохранить Выделенное как...", ""))
            {
                ExecCommand(COMMAND_SAVE_SELECTION);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::Separator();
            if (ImGui::BeginMenu("Open Recent"_RU >> u8"Открыть последние", ""))
            {
                for (auto& str: EPrefs->scene_recent_list)
                {
                    if (ImGui::MenuItem(str.c_str(), ""))
                    {
                        ExecCommand(COMMAND_LOAD, str);
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                ImGui::EndMenu();
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::Separator();
            if (ImGui::MenuItem("Quit"_RU >> u8"Выход", ""))
            {
                ExecCommand(COMMAND_QUIT);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::EndMenu();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        // --------------------------------------------------------------------------------------------
        ImGui::SameLine();
        ImGui::Separator();
        ImGui::Separator();
        ImGui::SameLine();
        // --------------------------------------------------------------------------------------------
        // Сцена
        if (ImGui::BeginMenu("Scene"_RU >> u8"Сцена"))
        {
            if (ImGui::MenuItem("Validate"_RU >> u8"Проверить", ""))
            {
                ExecCommand(COMMAND_VALIDATE_SCENE);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::Separator();
            if (ImGui::MenuItem("Summary Info"_RU >> u8"Сумарная информация", ""))
            {
                ExecCommand(COMMAND_CLEAR_SCENE_SUMMARY);
                ExecCommand(COMMAND_COLLECT_SCENE_SUMMARY);
                ExecCommand(COMMAND_SHOW_SCENE_SUMMARY);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::MenuItem("Highlight Texture..."_RU >> u8"Текстуры высокой чёткости...", ""))
            {
                ExecCommand(COMMAND_SCENE_HIGHLIGHT_TEXTURE);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::Separator();
            if (ImGui::MenuItem("Clear Debug Draw"_RU >> u8"Очистить отладочные метки", ""))
            {
                ExecCommand(COMMAND_CLEAR_DEBUG_DRAW);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::Separator();
            if (ImGui::MenuItem("Export entire Scene as Obj"_RU >> u8"Экспорт сцены в Obj", ""))
            {
                Scene->ExportObj(false);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::MenuItem("Export selection as Obj"_RU >> u8"Экспорт выделенного в Obj", ""))
            {
                Scene->ExportObj(true);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::EndMenu();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        // --------------------------------------------------------------------------------------------
        ImGui::SameLine();
        ImGui::Separator();
        ImGui::Separator();
        ImGui::SameLine();
        // --------------------------------------------------------------------------------------------
        // Compile
        if (ImGui::BeginMenu("Compile"_RU >> u8"Сборка Локации"))
        {
            if (ImGui::BeginMenu("Make All"_RU >> u8"Обновление всего"))
            {
                if (ImGui::MenuItem("Build"_RU >> u8"Компиляция", ""))
                {
                    ExecCommand(COMMAND_BUILD);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::Separator();
                if (ImGui::MenuItem("Make Game"_RU >> u8"Дополнить Game", ""))
                {
                    ExecCommand(COMMAND_MAKE_GAME);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                if (ImGui::MenuItem("Make Details"_RU >> u8"Дополнить Траву", ""))
                {
                    ExecCommand(COMMAND_MAKE_DETAILS);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                if (ImGui::MenuItem("Make Hom"_RU >> u8"Дополнить HOM", ""))
                {
                    ExecCommand(COMMAND_MAKE_HOM);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                if (ImGui::MenuItem("Make SOM"_RU >> u8"Дополнить SOM", ""))
                {
                    ExecCommand(COMMAND_MAKE_SOM);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                if (ImGui::MenuItem("Make AI-Map"_RU >> u8"Дополнить AI-Сетку", ""))
                {
                    ExecCommand(COMMAND_MAKE_AIMAP);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::EndMenu();
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            bool bDisable = false;
            if (LTools->IsCompilerRunning() || LTools->IsGameRunning())
            {
                ImGui::BeginDisabled();
                bDisable = true;
            }
            if (ImGui::BeginMenu("Compile"_RU >> u8"Компиляторы"))
            {
                if (ImGui::MenuItem("Geometry & Light"_RU >> u8"Геометрия и Свет", ""))
                {
                    LTools->RunXrLC();
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                if (ImGui::MenuItem("Detail Object Light"_RU >> u8"Освещение детальных объектов", ""))
                {
                    LTools->RunXrDO();
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                if (ImGui::BeginMenu("AIMap"_RU >> u8"AI-Сетка"))
                {
                    if (ImGui::MenuItem("High"_RU >> u8"High", ""))
                    {
                        LTools->RunXrAI_AIMap(false);
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    if (ImGui::MenuItem("Low"_RU >> u8"Low", ""))
                    {
                        LTools->RunXrAI_AIMap(false);
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    if (ImGui::MenuItem("Verify"_RU >> u8"Verify", ""))
                    {
                        LTools->RunXrAI_Verify();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    ImGui::EndMenu();
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                if (ImGui::BeginMenu("Spawn"_RU >> u8"Спавн"))
                {
                    if (ImGui::MenuItem("Only current level"_RU >> u8"Только текущий уровень", ""))
                    {
                        LTools->RunXrAI_Spawn(true);
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    if (ImGui::MenuItem("All levels"_RU >> u8"Все уровни", ""))
                    {
                        LTools->RunXrAI_Spawn(false);
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    ImGui::EndMenu();
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

                ImGui::EndMenu();
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (bDisable)
            {
                ImGui::EndDisabled();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Import Error List"_RU >> u8"Импорт списка ошибок", ""))
            {
                ExecCommand(COMMAND_IMPORT_COMPILER_ERROR);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::MenuItem("Import xrAI Error List"_RU >> u8"Импорт списка ошибок xrAI", ""))
            {
                ExecCommand(COMMAND_IMPORT_AICOMPILER_ERROR);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::MenuItem("Export Error List"_RU >> u8"Экспорт списка ошибок", ""))
            {
                ExecCommand(COMMAND_EXPORT_COMPILER_ERROR);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::MenuItem("Clear Error List"_RU >> u8"Очистить список ошибок", ""))
            {
                ExecCommand(COMMAND_CLEAR_DEBUG_DRAW);
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
        // --------------------------------------------------------------------------------------------
        // Эдиторы
        if (ImGui::BeginMenu("Editors"_RU >> u8"Эдиторы"))
        {
            if (ImGui::BeginMenu("Objects"_RU >> u8"Объекты"))
            {
                if (ImGui::MenuItem("Reload"_RU >> u8"Обновить Объекты"))
                {
                    ExecCommand(COMMAND_RELOAD_OBJECTS);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::Separator();
                if (ImGui::MenuItem("Library editor"_RU >> u8"Редактор Объектов"))
                {
                    ExecCommand(COMMAND_LIBRARY_EDITOR);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::EndMenu();
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::BeginMenu("Images"_RU >> u8"Текстуры"))
            {
                if (ImGui::MenuItem("Image Editor"_RU >> u8"Редактор Текстур", ""))
                {
                    ExecCommand(COMMAND_IMAGE_EDITOR);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::Separator();
                if (ImGui::MenuItem("Reload Textures"_RU >> u8"Обновить Текстуры в сцене", ""))
                {
                    ExecCommand(COMMAND_RELOAD_TEXTURES);
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
                ImGui::Separator();
                if (ImGui::MenuItem("Sync THM"_RU >> u8"Синхронизировать THM", ""))
                {
                    FS_FileSet files;
                    FS.file_list(files, _textures_, FS_ListFiles, "*.thm");
                    FS_FileSet::iterator I = files.begin();
                    FS_FileSet::iterator E = files.end();

                    for (; I != E; ++I)
                    {
                        ETextureThumbnail* TH = xr_new<ETextureThumbnail>((*I).name.c_str(), false);
                        TH->Load((*I).name.c_str(), _textures_);
                        TH->Save();
                        xr_delete(TH);
                    }
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
                ImGui::Separator();
                if (ImGui::MenuItem("Refresh Environment Library"_RU >> u8"Обновить библиотеку параметров", ""))
                {
                    ExecCommand(COMMAND_REFRESH_SOUND_ENVS);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                if (ImGui::MenuItem("Refresh Environment Geometry"_RU >> u8"Обновить параметры геометрии", ""))
                {
                    ExecCommand(COMMAND_REFRESH_SOUND_ENV_GEOMETRY);
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
        // --------------------------------------------------------------------------------------------
        ImGui::SameLine();
        ImGui::Separator();
        ImGui::Separator();
        ImGui::SameLine();
        // --------------------------------------------------------------------------------------------
        // Опции
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
            // --------------------------------------------------------------------------------------------
            // Погода
            {
                if (ImGui::BeginMenu("Environment"_RU >> u8"Погода"))
                {
                    {
                        if (ImGui::Button("Weather properties"_RU >> u8"Свойства погоды"))
                        {
                            ExecCommand(COMMAND_WEATHER_PROPERTIES);
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    }
                    ImGui::Separator();
                    bool selected = !psDeviceFlags.test(rsEnvironment);
                    if (ImGui::MenuItem("None", "", &selected))
                    {
                        psDeviceFlags.set(rsEnvironment, false);
                        g_pGamePersistent->Environment().Invalidate();
                        UI->RedrawScene();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    ImGui::Separator();
                    for (auto& i: g_pGamePersistent->Environment().WeatherCycles)
                    {
                        #pragma TODO("Romann: Если в UI_MainCommand.cpp исправится применение погодного цикла из ini файла - то тут надо заменить 'EPrefs->sWeather' на 'g_pGamePersistent->Environment().CurrentCycleName'")
                        selected = psDeviceFlags.test(rsEnvironment) && i.first == EPrefs->sWeather;
                        if (ImGui::MenuItem(i.first.c_str(), "", &selected))
                        {
                            psDeviceFlags.set(rsEnvironment, true);
                            g_pGamePersistent->Environment().SetWeather(i.first.c_str(), true);
                            EPrefs->sWeather = g_pGamePersistent->Environment().CurrentCycleName;
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    }
                    ImGui::Separator();
                    if (ImGui::Button("Reload"_RU >> u8"Перезагрузить"))
                    {
                        Engine.ReloadSettings();
                        g_pGamePersistent->Environment().ED_Reload();
                        UI->RedrawScene();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    ImGui::EndMenu();
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            }
            // --------------------------------------------------------------------------------------------
            ImGui::Separator();
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
        // --------------------------------------------------------------------------------------------
        ImGui::SameLine();
        ImGui::Separator();
        ImGui::Separator();
        ImGui::SameLine();
        // --------------------------------------------------------------------------------------------
        // Окна
        if (ImGui::BeginMenu("Windows"_RU >> u8"Окна"))
        {
            {
                bool selected = UIObjectList::IsOpen();
                if (ImGui::MenuItem("Object List"_RU >> u8"Список Объектов", "", &selected))
                {
                    if (selected)
                        UIObjectList::Show();
                    else
                        UIObjectList::Close();
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            }
            {
                bool selected = !MainForm->GetPropertiesFrom()->IsClosed();
                if (ImGui::MenuItem("Properties"_RU >> u8"Свойства", "", &selected))
                {
                    if (selected)
                        MainForm->GetPropertiesFrom()->Open();
                    else
                        MainForm->GetPropertiesFrom()->Close();
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            }
            {
                bool selected = !MainForm->GetWorldPropertiesFrom()->IsClosed();
                if (ImGui::MenuItem("World Properties"_RU >> u8"Мировые свойства", "", &selected))
                {
                    if (selected)
                        MainForm->GetWorldPropertiesFrom()->Open();
                    else
                        MainForm->GetWorldPropertiesFrom()->Close();
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            }
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
        // --------------------------------------------------------------------------------------------
        ImGui::SameLine();
        ImGui::Separator();
        ImGui::SameLine(0, 5);
        ImGui::Separator();
        ImGui::SameLine(0, 5);
        ImGui::Separator();
        ImGui::SameLine();
        // --------------------------------------------------------------------------------------------
        // Язык
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
        // --------------------------------------------------------------------------------------------
        ImGui::Separator();
        ImGui::EndMainMenuBar();
    }
}
