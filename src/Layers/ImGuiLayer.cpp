#include "Layers/ImGuiLayer.hpp"

namespace Ham
{
void ImGuiLayer::start()
{
    ImGui::SetCurrentContext(_integration.context());


    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    // io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
    // io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;
    io.ConfigWindowsMoveFromTitleBarOnly = true;


    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}

void ImGuiLayer::exit()
{
}


void ImGuiLayer::begin()
{
    _integration.newFrame();
    ImGuizmo::BeginFrame();
}

void ImGuiLayer::renderUI(Magnum::GL::Texture2D& texture)
{

    {
        ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Scene");
        auto     viewpoerPanelSize = ImGui::GetContentRegionAvail();
        Vector2i size((int)viewpoerPanelSize.x, (int)viewpoerPanelSize.y);


        auto& camera       = _scene->GetEntity("Main Camera").GetComponent<Component::Camera>();
        camera.aspectRatio = Vector2{size}.aspectRatio();

        // ImGui::Image((ImTextureID)_frameBufferColor.id(), ImVec2{(float)size.x(), (float)size.y()}, ImVec2{0, 1}, ImVec2{1, 0});
        Magnum::ImGuiIntegration::image(texture, Vector2{size});

        auto& cube = _scene->GetEntity("Cube");
        if (cube)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

            auto& cameraEntity    = _scene->GetEntity("Main Camera");
            auto& camera          = cameraEntity.GetComponent<Component::Camera>();
            auto& cameraTransform = cameraEntity.GetComponent<Component::Transform>();

            auto cameraView       = cameraTransform.ToMatrix().inverted();
            auto cameraProjection = camera.getProjectionMatrix();


            auto& entityTransform = cube.GetComponent<Component::Transform>().ToMatrix();

            if (ImGuizmo::Manipulate(cameraView.data(), cameraProjection.data(), ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::LOCAL, entityTransform.data()))
            {
            }
        }

        ImGui::End();
        ImGui::PopStyleVar();
    }





    {
        ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);

        ImGui::Begin("Debug");

        ImGui::Text("Hello, world!");

        static float _floatValue = 0;
        ImGui::SliderFloat("Float", &_floatValue, 0.0f, 1.0f);

        static Magnum::Color3 _clearColor;
        if (ImGui::ColorEdit3("Clear Color", _clearColor.data()))
            GL::Renderer::setClearColor(_clearColor);

        auto& material = _scene->GetEntity("Cube").GetComponent<Component::Material>();
        ImGui::ColorEdit3("Cube Color", material.color.data());

        // if (ImGui::Button("Test Window"))
        //     _showDemoWindow ^= true;

        // if (ImGui::Button("Another Window"))
        //     _showAnotherWindow ^= true;

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0 / Double(ImGui::GetIO().Framerate), Double(ImGui::GetIO().Framerate));

        auto& camera = _scene->GetEntity("Main Camera").GetComponent<Component::Transform>();

        ImGui::Text("Camera");

        if (ImGui::DragFloat3("Position", camera.position.data()))
        {
            HAM_INFO("Position is: {0} {1} {2}", camera.position.x(), camera.position.y(), camera.position.z());
        }

        if (ImGui::DragFloat3("Rotation", camera.rotation.data()))
        {
            HAM_INFO("Rotation is: {0} {1} {2}", camera.rotation.x(), camera.rotation.y(), camera.rotation.z());
        }

        if (ImGui::DragFloat3("Scale", camera.scale.data()))
        {
            camera.scale.x() = Math::max(0.0f, camera.scale.x());
            camera.scale.y() = Math::max(0.0f, camera.scale.y());
            camera.scale.z() = Math::max(0.0f, camera.scale.z());
            HAM_INFO("Scale is: {0} {1} {2}", camera.scale.x(), camera.scale.y(), camera.scale.z());
        }
        ImGui::End();
    }

    // if (_showAnotherWindow)
    // {
    //     ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_FirstUseEver);
    //     ImGui::Begin("Another Window", &_showAnotherWindow);
    //     ImGui::Text("Hello");
    //     ImGui::End();
    // }

    // if (_showDemoWindow)
    // {
    //     ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
    //     ImGui::ShowDemoWindow();
    // }

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        HAM_INFO("ImGui: ViewportsEnable");
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void ImGuiLayer::end()
{
    _integration.drawFrame();
}
} // namespace Ham