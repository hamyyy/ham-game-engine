#pragma once

#include <imgui_internal.h>

namespace Ham
{
namespace Gizmo
{
static void DrawTransformGizmo(Entity entity, Entity camera, ImGuizmo::OPERATION operation, float* snapValue = nullptr) // function must be called inside an ImGui window
{
    if (entity)
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        float  titleBarHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().FramePadding.y * 2.0f;
        ImRect windowRect     = ImRect({ImGui::GetWindowPos().x, ImGui::GetWindowPos().y}, ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + ImGui::GetWindowHeight()));
        ImGuizmo::SetRect(windowRect.Min.x, windowRect.Min.y, windowRect.Max.x - windowRect.Min.x, windowRect.Max.y - windowRect.Min.y);

        auto& cameraComponent = camera.GetComponent<Component::Camera>();
        auto& cameraTransform = camera.GetComponent<Component::Transform>();

        auto& entityTransform = entity.GetComponent<Component::Transform>();

        auto cameraView       = cameraTransform.ToMatrix().inverted();
        auto cameraProjection = cameraComponent.getProjectionMatrix();

        float entityTransformMatrix[4 * 4];
        float deltaMatrix[4 * 4];

        ImGuizmo::RecomposeMatrixFromComponents(entityTransform.position.data(), (Float*)entityTransform.rotation.data(), entityTransform.scale.data(), entityTransformMatrix);

        if (ImGuizmo::Manipulate(cameraView.data(), cameraProjection.data(), operation, ImGuizmo::MODE::WORLD, entityTransformMatrix, deltaMatrix, snapValue))
        {
            if (ImGui::IsWindowFocused())
                ImGuizmo::DecomposeMatrixToComponents(entityTransformMatrix, entityTransform.position.data(), (Float*)entityTransform.rotation.data(), entityTransform.scale.data());
        }
    }
}
} // namespace Gizmo


} // namespace Ham
