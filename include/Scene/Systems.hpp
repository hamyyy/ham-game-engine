#pragma once

#include "Core/Core.hpp"
#include <imgui_internal.h>


using namespace Math::Literals;

namespace Ham::System
{
namespace Helper
{
inline static void PrintMatrix4(Magnum::Matrix4 mat)
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

    auto view = scene->GetComponent<Component::Transform>();
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

    auto view = scene->GetComponent<Component::Transform>();
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

    auto view = scene->GetComponents<Component::Transform, Component::Material, Component::Mesh>();
    for (auto entity : view)
    {
        auto [transform, material, mesh] = view.get<Component::Transform, Component::Material, Component::Mesh>(entity);

        // Magnum::GL::Mesh mesh;

        // mesh
        //     .setCount(meshComponent.indexCount)
        //     .addVertexBuffer(std::move(meshComponent.vertexBuffer), 0, Shaders::PhongGL::Position{}, Shaders::PhongGL::Normal{})
        //     .setIndexBuffer(std::move(meshComponent.indexBuffer), 0, meshComponent.indexType);
        material
            .setLightPositions({{1.4f, 1.0f, 0.75f, 0.0f}})
            .setDiffuseColor(material.color)
            .setAmbientColor(Magnum::Color3::fromHsv({material.color.hue(), material.color.saturation(), material.color.value() * 0.5f}))
            .setTransformationMatrix(transform.ToMatrix())
            .setNormalMatrix(transform.ToMatrix().normalMatrix())
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
    auto view = scene->GetComponent<Component::Gui::GLWindow>();
    for (auto entity : view)
    {

        auto& frameBuffer = view.get<Component::Gui::GLWindow>(entity).frameBuffer;
        auto& colorBuffer = view.get<Component::Gui::GLWindow>(entity).colorBuffer;
        auto& depthBuffer = view.get<Component::Gui::GLWindow>(entity).depthBuffer;
        auto& size        = view.get<Component::Gui::GLWindow>(entity).size;

        if (size.x() > frameBuffer.viewport().size().x() || size.y() > frameBuffer.viewport().size().y())
        {
            frameBuffer = GL::Framebuffer({{}, size});
            colorBuffer = Magnum::GL::Texture2D();
            depthBuffer = Magnum::GL::Renderbuffer();

            frameBuffer.attachTexture(GL::Framebuffer::ColorAttachment{0}, colorBuffer, 0);
            frameBuffer.attachRenderbuffer(GL::Framebuffer::BufferAttachment::DepthStencil, depthBuffer);

            colorBuffer.setStorage(1, GL::TextureFormat::RGBA8, size);
            depthBuffer.setStorage(GL::RenderbufferFormat::Depth24Stencil8, size);
        }
    }
}

inline static void RenderFrameBuffer(Scene* scene)
{
    auto view = scene->GetComponent<Component::Gui::GLWindow>();
    for (auto entity : view)
    {
        auto& colorBuffer = view.get<Component::Gui::GLWindow>(entity).colorBuffer;
        auto& size        = view.get<Component::Gui::GLWindow>(entity).size;
        auto& title       = view.get<Component::Gui::GLWindow>(entity).title;

        ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        if (ImGui::Begin(title.c_str(), nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
        {
            auto viewpoerPanelSize = ImGui::GetContentRegionAvail();
            size                   = Vector2i((int)viewpoerPanelSize.x, (int)viewpoerPanelSize.y);

            Magnum::ImGuiIntegration::image(colorBuffer, Vector2{size});
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }
}

inline static void ProcessEventPassthrough(Scene* scene, Vector2i mouseDownPos)
{
    auto view = scene->GetComponent<Component::Gui::GLWindow>();
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
