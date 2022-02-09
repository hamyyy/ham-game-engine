#pragma once

#include "Core/Core.hpp"
#include <imgui_internal.h>

namespace fs = std::filesystem;

using namespace Math::Literals;

namespace Ham::System
{
namespace Helper
{
inline static void PrintMatrix4(Matrix4 mat)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << mat[i][j] << ", ";
        }
        std::cout << "\n";
    }
}
} // namespace Helper

inline static void Print(Scene* scene)
{
    // for (auto& [entity, pos] : scene->GetComponent<Component::Transform>().each())
    // {
    //     std::cout << "position: (" << pos.x << ", " << pos.y << ")" << std::endl;
    // }

    auto view = scene->GetComponentView<Component::Transform>();
    for (auto entity : view)
    {
        auto& transform = view.get<Component::Transform>(entity);

        std::cout << "position:" << std::endl;
        Helper::PrintMatrix4(transform.ToMatrix());
        std::cout << std::endl;
    }
}

inline static void Move(Scene* scene)
{
    // for (auto& [entity, pos] : scene->GetComponent<Component::Position>().each())
    // {
    //     pos.x += 0.1f;
    //     pos.y += 0.1f;
    // }

    auto view = scene->GetComponentView<Component::Transform>();
    for (auto entity : view)
    {
        auto& transform = view.get<Component::Transform>(entity);

        transform.scale *= {1.0001f, 1.0001f, 1.0001f};
    }
}

inline static void Draw(Scene* scene, Entity camera)
{
    if (!camera) return;

    auto cameraTransform = camera.GetComponent<Component::Transform>();
    // cameraTransform.scale = {1.0f, 1.0f, 1.0f};
    auto projectionMatrix = camera.GetComponent<Component::Camera>().getProjectionMatrix() * cameraTransform.ToMatrix().inverted();

    auto view = scene->GetComponentGroup<Component::Transform, Component::ShaderMaterial, Component::Mesh>();
    for (auto entity : view)
    {
        auto [transform, material, mesh] = view.get<Component::Transform, Component::ShaderMaterial, Component::Mesh>(entity);

        material.shader
            .setDiffuseColor(material.color)
            .setAmbientColor(Color3::fromHsv({material.color.hue(), material.color.saturation(), material.color.value() * 0.5f}))
            .setLightPositions({{1.4f, 1.0f, 0.75f, 0.0f}})
            .setTransformationMatrix(transform.ToMatrix())
            .setNormalMatrix(transform.ToMatrix().normalMatrix())
            .setProjectionMatrix(projectionMatrix)
            .draw(mesh);
    }
}

inline static void DebugDraw(Scene* scene, Entity camera)
{
    if (!camera) return;

    auto cameraTransform = camera.GetComponent<Component::Transform>();
    // cameraTransform.scale = {1.0f, 1.0f, 1.0f};
    auto projectionMatrix = camera.GetComponent<Component::Camera>().getProjectionMatrix() * cameraTransform.ToMatrix().inverted();

    // auto shader = Shaders::MeshVisualizerGL3D{Shaders::MeshVisualizerGL3D::Flag::Wireframe | Shaders::MeshVisualizerGL3D::Flag::NoGeometryShader};

    auto view = scene->GetComponentView<Component::Transform, Component::MeshVisualizerShaderMaterial, Component::DebugMesh>();
    for (auto entity : view)
    {
        auto [transform, material, mesh] = view.get<Component::Transform, Component::MeshVisualizerShaderMaterial, Component::DebugMesh>(entity);

        material.shader
            .setColor(material.color)
            .setWireframeColor(material.wireframeColor)
            .setWireframeWidth(material.wireframeWidth)
            .setTransformationMatrix(transform.ToMatrix())
            .setProjectionMatrix(projectionMatrix)
            .draw(mesh);
    }
}

// inline static void Draw(Scene* scene, uint32_t cameraIndex = 0) {
//     auto camera = scene->GetEditorCamera(cameraIndex);
//     Draw(scene, camera);
// }

inline static void UpdateFrameBuffer(Scene* scene)
{
    auto view = scene->GetComponentView<Component::Gui::GLWindow>();
    for (auto entity : view)
    {

        auto& frameBuffer = view.get<Component::Gui::GLWindow>(entity).frameBuffer;
        auto& colorBuffer = view.get<Component::Gui::GLWindow>(entity).colorBuffer;
        auto& depthBuffer = view.get<Component::Gui::GLWindow>(entity).depthBuffer;
        auto& size        = view.get<Component::Gui::GLWindow>(entity).size;

        if (size.x() > frameBuffer.viewport().size().x() || size.y() > frameBuffer.viewport().size().y())
        {
            frameBuffer = GL::Framebuffer({{}, size});
            colorBuffer = GL::Texture2D();
            depthBuffer = GL::Renderbuffer();

            frameBuffer.attachTexture(GL::Framebuffer::ColorAttachment{0}, colorBuffer, 0);
            frameBuffer.attachRenderbuffer(GL::Framebuffer::BufferAttachment::DepthStencil, depthBuffer);

            colorBuffer.setStorage(1, GL::TextureFormat::RGBA8, size);
            depthBuffer.setStorage(GL::RenderbufferFormat::Depth24Stencil8, size);
        }
    }
}

inline static void DrawManipulateGizmo(Scene* scene, Entity camera)
{
    auto selected = scene->GetSelectedEntity();

    Settings::useSnap = Input::IsKeyPressed(Key::LeftCtrl);

    if (Settings::manipulateOperation == ImGuizmo::OPERATION::ROTATE)
        Settings::currentSnapValue = Settings::angularSnapValue;
    else
        Settings::currentSnapValue = Settings::linearSnapValue;

    Vector3 snapValue = {Settings::currentSnapValue, Settings::currentSnapValue, Settings::currentSnapValue};

    if (selected)
        Gizmo::DrawTransformGizmo(selected, camera, Settings::manipulateOperation, Settings::useSnap ? snapValue.data() : nullptr);
}


inline static void RenderGizmos(Scene* scene, Entity camera)
{
    auto cameraComponent = camera.GetComponent<Component::Camera>();
    auto proj            = cameraComponent.getProjectionMatrix();
    auto cameraTransform = camera.GetComponent<Component::Transform>();

    auto view = scene->GetComponentView<Component::Transform, Component::Gui::Gizmo>();
    for (auto entity : view)
    {
        Entity currentEntity{entity, scene};

        auto [transform, gizmo] = view.get<Component::Transform, Component::Gui::Gizmo>(entity);

        if (gizmo.image != nullptr)
        {
            if ((transform.position - cameraTransform.position).length() < cameraComponent.nearPlane) continue;

            auto matrix = proj * cameraTransform.ToMatrix().inverted();
            auto pos    = transform.position;
            pos         = matrix.transformPoint(pos);

            auto    windowMin  = ImGui::GetItemRectMin();
            auto    windowMax  = ImGui::GetItemRectMax();
            Vector2 windowSize = {windowMax.x - windowMin.x, windowMax.y - windowMin.y};
            Vector2 size       = {50.0f, 50.0f};

            auto cameraToEntityVector = (transform.position - cameraTransform.position).normalized();
            auto cameraForwardVector  = cameraTransform.ToMatrix().transformVector(-Vector3::zAxis()).normalized();
            if (Math::dot(cameraForwardVector, cameraToEntityVector) > 0.0f)
            {
                ImGui::SetCursorPos(ImVec2{((pos.x() + 1.f) * 0.5f * windowSize.x()) - (size.x() / 2), ((-pos.y() + 1.f) * 0.5f * windowSize.y()) - (size.y() / 2)});

                ImGuiIntegration::image(*gizmo.image, size);
                if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                {
                    scene->SetSelectedEntity(currentEntity);
                }
            }

            DrawManipulateGizmo(scene, camera);
        }
    }
}



inline static void GenerateGizmoIcons(Scene* scene)
{

    auto view = scene->GetComponentView<Component::Gui::Gizmo>();
    for (auto entity : view)
    {
        auto& gizmo = view.get<Component::Gui::Gizmo>(entity);
        if (gizmo.image == nullptr)
        {
            if (gizmo.path.empty())
            {
                HAM_CORE_WARN("Gizmo path is empty");
                continue;
            }

            auto doc = lunasvg::Document::loadFromFile(gizmo.path);

            if (doc == nullptr)
            {
                HAM_CORE_WARN("Failed to load gizmo file: {0}", gizmo.path);
                continue;
            }

            auto bitmap = doc->renderToBitmap(gizmo.imageData.size().x(), gizmo.imageData.size().y(), 0x00000000);

            if (!bitmap.valid())
            {
                HAM_CORE_WARN("Failed to render gizmo file: {0}", gizmo.path);
                continue;
            }

            gizmo.imageData.setData({bitmap.data(), bitmap.width() * bitmap.height() * 4});

            gizmo.image = new GL::Texture2D();

            gizmo.image->setStorage(1, GL::textureFormat(gizmo.imageData.format()), gizmo.imageData.size());

            gizmo.image->setSubImage(0, {}, gizmo.imageData);
        }

        // ImGui::SetCursorPos(ImVec2{0, 0});
        // ImGuiIntegration::imageButton(*gizmo.image, Vector2{gizmo.size});
    }
}

inline static void RenderFrameBuffer(Scene* scene)
{
    auto view = scene->GetComponentView<Component::Gui::GLWindow>();
    for (auto entity : view)
    {
        auto& colorBuffer = view.get<Component::Gui::GLWindow>(entity).colorBuffer;
        auto& size        = view.get<Component::Gui::GLWindow>(entity).size;
        auto& title       = view.get<Component::Gui::GLWindow>(entity).title;
        auto& camera      = view.get<Component::Gui::GLWindow>(entity).camera;

        ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        if (ImGui::Begin(title.c_str(), nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
        {
            auto viewpoerPanelSize = ImGui::GetContentRegionAvail();
            size                   = Vector2i((int)viewpoerPanelSize.x, (int)viewpoerPanelSize.y);

            ImGuiIntegration::image(colorBuffer, Vector2{size});
            RenderGizmos(scene, camera);
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }
}


inline static void ProcessEventPassthrough(Scene* scene, Vector2i mouseDownPos)
{
    auto view = scene->GetComponentView<Component::Gui::GLWindow>();
    for (auto entity : view)
    {
        auto& title             = view.get<Component::Gui::GLWindow>(entity).title;
        auto& isFocused         = view.get<Component::Gui::GLWindow>(entity).isFocused;
        auto& isTitlebarDragged = view.get<Component::Gui::GLWindow>(entity).isTitlebarDragged;
        auto& isOpen            = view.get<Component::Gui::GLWindow>(entity).isOpen;

        isOpen = false;
        if (ImGui::Begin(title.c_str()))
        {
            isOpen = true;

            float  titleBarHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().FramePadding.y * 2.0f;
            ImRect titleBarRect   = ImRect(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + titleBarHeight));

            ImRect rightEdgeRect  = ImRect({ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - 5, ImGui::GetWindowPos().y}, ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + ImGui::GetWindowHeight()));
            ImRect leftEdgeRect   = ImRect(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + 5, ImGui::GetWindowPos().y + ImGui::GetWindowHeight()));
            ImRect topEdgeRect    = ImRect(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + 5));
            ImRect bottomEdgeRect = ImRect({ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - 5}, ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + ImGui::GetWindowHeight()));

            ImRect bottomRightCornerRect = ImRect({ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - 5, ImGui::GetWindowPos().y}, ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + ImGui::GetWindowHeight()));
            ImRect bottomLeftCornerRect  = ImRect({ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - 5}, ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + ImGui::GetWindowHeight()));

            if (titleBarRect.Contains(ImVec2((float)mouseDownPos.x(), (float)mouseDownPos.y())) ||
                rightEdgeRect.Contains(ImVec2((float)mouseDownPos.x(), (float)mouseDownPos.y())) ||
                leftEdgeRect.Contains(ImVec2((float)mouseDownPos.x(), (float)mouseDownPos.y())) ||
                topEdgeRect.Contains(ImVec2((float)mouseDownPos.x(), (float)mouseDownPos.y())) ||
                bottomEdgeRect.Contains(ImVec2((float)mouseDownPos.x(), (float)mouseDownPos.y())) ||
                bottomRightCornerRect.Contains(ImVec2((float)mouseDownPos.x(), (float)mouseDownPos.y())) ||
                bottomLeftCornerRect.Contains(ImVec2((float)mouseDownPos.x(), (float)mouseDownPos.y()))) isTitlebarDragged = true; //(pos.x > titleBarRect.Min.x && pos.x < titleBarRect.Max.x && pos.y > titleBarRect.Min.y && pos.y < titleBarRect.Max.y);
            if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) isTitlebarDragged = false;
        }
        isFocused = ImGui::IsWindowFocused() && isOpen && !ImGui::IsWindowCollapsed() && !(ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0) && isTitlebarDragged);
        ImGui::End();
    }
}
} // namespace Ham::System
