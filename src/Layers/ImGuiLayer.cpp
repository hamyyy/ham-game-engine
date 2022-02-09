#include "Layers/ImGuiLayer.hpp"

#include <imgui_internal.h>

namespace Ham
{

ImGuiLayer::ImGuiLayer(Scene* scene) :
    _scene(scene),
    Layer("ImGuiLayer")
{
}

ImGuiLayer::ImGuiLayer(Scene* scene, const Vector2& size, const Vector2i& windowSize, const Vector2i& framebufferSize) :
    _scene(scene),
    _integration(size, windowSize, framebufferSize),
    Layer("ImGuiLayer")
{
}

ImGuiLayer::~ImGuiLayer() {}

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
}

void ImGuiLayer::exit()
{
}


void ImGuiLayer::begin()
{
    _integration.newFrame();
    ImGuizmo::BeginFrame();
    // ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}

void ImGuiLayer::renderUI() // GL::Texture2D& texture
{
}

void ImGuiLayer::end()
{
    _integration.drawFrame();
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        HAM_INFO("ImGui: ViewportsEnable");
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

static bool isAnyGLWindowFocused(Scene* scene)
{
    auto view = scene->GetComponentView<Component::Gui::GLWindow>();
    for (auto entity : view)
    {
        if (view.get<Component::Gui::GLWindow>(entity).isFocused) return true;
    }
    return false;
}


void ImGuiLayer::viewportEvent(Platform::Application::ViewportEvent& event)
{
    _integration.relayout(Vector2{event.windowSize()} / event.dpiScaling(), event.windowSize(), event.framebufferSize());
}

void ImGuiLayer::keyPressEvent(Platform::Application::KeyEvent& event)
{
    _integration.handleKeyPressEvent(event);
    if (!isAnyGLWindowFocused(_scene)) event.setAccepted();
}

void ImGuiLayer::keyReleaseEvent(Platform::Application::KeyEvent& event)
{
    _integration.handleKeyReleaseEvent(event);
    if (!isAnyGLWindowFocused(_scene)) event.setAccepted();
}

void ImGuiLayer::mousePressEvent(Platform::Application::MouseEvent& event)
{
    _mouseDownPos = event.position();
    _integration.handleMousePressEvent(event);
    if (!isAnyGLWindowFocused(_scene)) event.setAccepted();
}

void ImGuiLayer::mouseReleaseEvent(Platform::Application::MouseEvent& event)
{
    _integration.handleMouseReleaseEvent(event);
    if (!isAnyGLWindowFocused(_scene)) event.setAccepted();
}

void ImGuiLayer::mouseMoveEvent(Platform::Application::MouseMoveEvent& event)
{
    // ImGui::GetID("#SCENE");
    // auto vp = ImGui::GetWindowViewport();
    // HAM_INFO("Scene Window Position: {0}x {1}y", vp->Pos.x, vp->Pos.y);
    // ImGui::PopID();

    _integration.handleMouseMoveEvent(event);
    if (!isAnyGLWindowFocused(_scene)) event.setAccepted();
}

void ImGuiLayer::mouseScrollEvent(Platform::Application::MouseScrollEvent& event)
{
    _integration.handleMouseScrollEvent(event);
    if (!isAnyGLWindowFocused(_scene)) event.setAccepted();
}

void ImGuiLayer::textInputEvent(Platform::Application::TextInputEvent& event)
{
    _integration.handleTextInputEvent(event);
    if (!isAnyGLWindowFocused(_scene)) event.setAccepted();
}
} // namespace Ham