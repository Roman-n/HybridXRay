#include "stdafx.h"
#include "UIMoveToForm.h"
#include "ui_main.h"

UIMoveToForm::UIMoveToForm()
{
    const Fvector& CameraCurrentPosition = EDevice->m_Camera.GetPosition();

    m_BackupCameraPosition               = CameraCurrentPosition;
    m_DesiredCameraPosition              = CameraCurrentPosition;
}

void UIMoveToForm::Draw()
{
    ImGui::SetNextWindowPos(ImGui::GetWindowViewport()->GetCenter(), ImGuiCond_Appearing, {0.5f, 0.5f});

    ImGui::Begin("Move to", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_AlwaysAutoResize
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoSavedSettings);
    ImGui::BeginGroup();
    ImGui::InputFloat("X", &m_DesiredCameraPosition.x, 0.1f, 1.f);
    ImGui::SameLine(0);
    if (ImGui::Button("Move"))
        CLBOk();
    ImGui::EndGroup();
    ImGui::BeginGroup();
    ImGui::InputFloat("Y", &m_DesiredCameraPosition.y, 0.1f, 1.f);
    ImGui::SameLine(0);
    if (ImGui::Button("Cancel"))
        CLBCancel();
    ImGui::EndGroup();
    ImGui::BeginGroup();
    ImGui::InputFloat("Z", &m_DesiredCameraPosition.z, 0.1f, 1.f);
    ImGui::SameLine(0);
    if (ImGui::Button("Reset"))
        CLBReset();
    ImGui::EndGroup();
    ImGui::End();
}

void UIMoveToForm::CLBOk()
{
    EDevice->m_Camera.Set(EDevice->m_Camera.GetHPB(), m_DesiredCameraPosition);
    UI->RedrawScene();
    // bOpen  = false;
}

void UIMoveToForm::CLBCancel()
{
    m_DesiredCameraPosition = m_BackupCameraPosition;
    bOpen                   = false;
}

void UIMoveToForm::CLBReset()
{
    m_DesiredCameraPosition = m_BackupCameraPosition;
}
