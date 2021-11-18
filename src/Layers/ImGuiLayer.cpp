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

void ImGuiLayer::renderUI() // Magnum::GL::Texture2D& texture
{
    static bool _showDemoWindow = true;


    {
        static bool show = true;
        if (show)
        {

            ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
            if (ImGui::Begin("Debug", &show))
            {
                ImGui::Text("Hello, world!");

                static float _floatValue = 0;
                ImGui::SliderFloat("Float", &_floatValue, 0.0f, 1.0f);

                static Magnum::Color3 _clearColor;
                if (ImGui::ColorEdit4("Clear Color", _clearColor.data()))
                    GL::Renderer::setClearColor(_clearColor);

                auto& material = _scene->GetEntity("Cube2").GetComponent<Component::Material>();
                ImGui::ColorEdit4("Cube Color", material.color.data());

                if (ImGui::Button(_showDemoWindow ? "Hide Demo Window" : "Show Demo Window"))
                    _showDemoWindow ^= true;

                if (ImGui::Button("Log"))
                {
                    HAM_CORE_INFO("This is an info");
                    HAM_CORE_TRACE("This is a trace");
                    HAM_CORE_WARN("This is a warning");
                    HAM_CORE_ERROR("This is an error");
                    HAM_CORE_CRITICAL("This is a critical");

                    HAM_INFO("This is an info");
                    HAM_TRACE("This is a trace");
                    HAM_WARN("This is a warning");
                    HAM_ERROR("This is an error");
                    HAM_CRITICAL("This is a critical");
                }

                if (ImGui::Button("Clear Console"))
                {
                    Log::GetLoggerStringStream().str("");
                    Log::GetLoggerStringStream().clear();
                }

                // if (ImGui::Button("Another Window"))
                //     _showAnotherWindow ^= true;

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0 / Double(ImGui::GetIO().Framerate), Double(ImGui::GetIO().Framerate));

                auto transformVisual = [&](Entity entity) {
                    auto  tag = entity.GetComponent<Component::Tag>().tag;
                    auto& t   = entity.GetComponent<Component::Transform>();

                    ImGui::Text(tag.c_str());

                    ImGui::PushID(tag.c_str());

                    ImGui::DragFloat3("Position", t.position.data(), 0.1f);

                    ImGui::DragFloat3("Rotation", (Magnum::Float*)t.rotation.data(), 0.1f);

                    ImGui::DragFloat3("Scale", t.scale.data(), 0.1f);

                    ImGui::PopID();
                };

                transformVisual(_scene->GetEditorCamera());
                transformVisual(_scene->GetEditorCamera(1));
                // transformVisual("Cube");
                transformVisual(_scene->GetEntity("Cube2"));
            }
            ImGui::End();
        }
    }

    // if (_showAnotherWindow)
    // {
    //     ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_FirstUseEver);
    //     ImGui::Begin("Another Window", &_showAnotherWindow);
    //     ImGui::Text("Hello");
    //     ImGui::End();
    // }

    if (_showDemoWindow)
    {
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
        ImGui::ShowDemoWindow(&_showDemoWindow);
    }



    {
        static bool show = true;
        if (show)
        {
            static int count = 0;
            ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
            if (ImGui::Begin("Console", &show))
            {

                if (ImGui::Button("Log"))
                {
                    HAM_INFO("This is an info {0}", count);
                }
            }

            ImGui::End();
        }
    }

    {
        // ImGui::Begin("Scene");
        // float  titleBarHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().FramePadding.y * 2.0f;
        // ImRect titleBarRect   = ImRect(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + titleBarHeight));

        // ImRect rightEdgeRect  = ImRect({ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - 5, ImGui::GetWindowPos().y}, ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + ImGui::GetWindowHeight()));
        // ImRect leftEdgeRect   = ImRect(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + 5, ImGui::GetWindowPos().y + ImGui::GetWindowHeight()));
        // ImRect topEdgeRect    = ImRect(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + 5));
        // ImRect bottomEdgeRect = ImRect({ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - 5}, ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + ImGui::GetWindowHeight()));

        // ImRect bottomRightCornerRect = ImRect({ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - 5, ImGui::GetWindowPos().y}, ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + ImGui::GetWindowHeight()));
        // ImRect bottomLeftCornerRect  = ImRect({ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - 5}, ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + ImGui::GetWindowHeight()));

        // if (titleBarRect.Contains(ImVec2((float)_mouseDownPos.x(), (float)_mouseDownPos.y())) ||
        //     rightEdgeRect.Contains(ImVec2((float)_mouseDownPos.x(), (float)_mouseDownPos.y())) ||
        //     leftEdgeRect.Contains(ImVec2((float)_mouseDownPos.x(), (float)_mouseDownPos.y())) ||
        //     topEdgeRect.Contains(ImVec2((float)_mouseDownPos.x(), (float)_mouseDownPos.y())) ||
        //     bottomEdgeRect.Contains(ImVec2((float)_mouseDownPos.x(), (float)_mouseDownPos.y())) ||
        //     bottomRightCornerRect.Contains(ImVec2((float)_mouseDownPos.x(), (float)_mouseDownPos.y())) ||
        //     bottomLeftCornerRect.Contains(ImVec2((float)_mouseDownPos.x(), (float)_mouseDownPos.y()))) _isTitlebarBeingDragged = true; //(pos.x > titleBarRect.Min.x && pos.x < titleBarRect.Max.x && pos.y > titleBarRect.Min.y && pos.y < titleBarRect.Max.y);
        // if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) _isTitlebarBeingDragged = false;

        // _sceneIsFocused = ImGui::IsWindowFocused() && !ImGui::IsWindowCollapsed() && !(ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0) && _isTitlebarBeingDragged);
        // ImGui::End();
        System::ProcessEventPassthrough(_scene, _mouseDownPos);
    }
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
    auto view = scene->GetComponent<Component::Gui::GLWindow>();
    for (auto entity : view)
    {
        if (view.get<Component::Gui::GLWindow>(entity).isFocused) return true;
    }
    return false;
}


void ImGuiLayer::viewportEvent(Magnum::Platform::Application::ViewportEvent& event)
{
    _integration.relayout(Vector2{event.windowSize()} / event.dpiScaling(), event.windowSize(), event.framebufferSize());
}

void ImGuiLayer::keyPressEvent(Magnum::Platform::Application::KeyEvent& event)
{
    _integration.handleKeyPressEvent(event);
    if (!isAnyGLWindowFocused(_scene)) event.setAccepted();
}

void ImGuiLayer::keyReleaseEvent(Magnum::Platform::Application::KeyEvent& event)
{
    _integration.handleKeyReleaseEvent(event);
    if (!isAnyGLWindowFocused(_scene)) event.setAccepted();
}

void ImGuiLayer::mousePressEvent(Magnum::Platform::Application::MouseEvent& event)
{
    _mouseDownPos = event.position();
    _integration.handleMousePressEvent(event);
    if (!isAnyGLWindowFocused(_scene)) event.setAccepted();
}

void ImGuiLayer::mouseReleaseEvent(Magnum::Platform::Application::MouseEvent& event)
{
    _integration.handleMouseReleaseEvent(event);
    if (!isAnyGLWindowFocused(_scene)) event.setAccepted();
}

void ImGuiLayer::mouseMoveEvent(Magnum::Platform::Application::MouseMoveEvent& event)
{
    // ImGui::GetID("#SCENE");
    // auto vp = ImGui::GetWindowViewport();
    // HAM_INFO("Scene Window Position: {0}x {1}y", vp->Pos.x, vp->Pos.y);
    // ImGui::PopID();

    _integration.handleMouseMoveEvent(event);
    if (!isAnyGLWindowFocused(_scene)) event.setAccepted();
}

void ImGuiLayer::mouseScrollEvent(Magnum::Platform::Application::MouseScrollEvent& event)
{
    _integration.handleMouseScrollEvent(event);
    if (!isAnyGLWindowFocused(_scene)) event.setAccepted();
}

void ImGuiLayer::textInputEvent(Magnum::Platform::Application::TextInputEvent& event)
{
    _integration.handleTextInputEvent(event);
    if (!isAnyGLWindowFocused(_scene)) event.setAccepted();
}
} // namespace Ham