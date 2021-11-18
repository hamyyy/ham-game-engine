#include "HamGameEngine.hpp"

using WindowFlag = Magnum::Platform::Application::Configuration::WindowFlag;

#define CONFIG_FLAGS (WindowFlag::Resizable)

namespace Ham
{


GameEngine::GameEngine(const Arguments& arguments) :
    Platform::Application{arguments,
                          Configuration{}
                              .setTitle("Magnum Primitives Example")
                              .addWindowFlags(CONFIG_FLAGS)
                              .setSize(Magnum::Vector2i{1280, 720})}
{
    Log::Initialize();
    LayerManager::Initialize();

    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

    GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add, GL::Renderer::BlendEquation::Add);
    GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::SourceAlpha, GL::Renderer::BlendFunction::OneMinusSourceAlpha);

    _imGuiLayer                   = new ImGuiLayer(&_scene);
    _imGuiLayer->GetIntegration() = Magnum::ImGuiIntegration::Context(Vector2{windowSize()} / dpiScaling(), windowSize(), framebufferSize());

    LayerManager::PushOverlay(_imGuiLayer);
    LayerManager::PushLayer(new RenderLayer(&_scene));
    LayerManager::PushLayer(new RenderLayer2(&_scene));
}

void GameEngine::drawEvent()
{
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth).bind();


    if (ImGui::GetIO().WantTextInput && !isTextInputActive())
        startTextInput();
    else if (!ImGui::GetIO().WantTextInput && isTextInputActive())
        stopTextInput();

    // _frameBuffer.setViewport({{}, size});
    // _frameBufferColor.setStorage(1, GL::TextureFormat::RGBA8, size);
    // _frameBufferDepthStencil.setStorage(GL::RenderbufferFormat::DepthComponent24, size);

    System::UpdateFrameBuffer(&_scene);

    LayerManager::forEachLayer([](Layer* layer) {
        layer->update();
    });

    GL::defaultFramebuffer.bind();

    GL::Renderer::enable(GL::Renderer::Feature::Blending);
    GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);
    GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);
    GL::Renderer::disable(GL::Renderer::Feature::DepthTest);

    _imGuiLayer->GetIntegration().updateApplicationCursor(*this);

    _imGuiLayer->begin();
    drawTitlebar();
    System::RenderFrameBuffer(&_scene);
    LayerManager::forEachLayer([](Layer* layer) {
        if (layer->isVisible())
            layer->renderUI();
    });
    _imGuiLayer->end();

    GL::Renderer::disable(GL::Renderer::Feature::Blending);
    GL::Renderer::disable(GL::Renderer::Feature::ScissorTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);

    swapBuffers();
    redraw();
}

void GameEngine::viewportEvent(ViewportEvent& event)
{
    GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});

    // _imGuiLayer->GetIntegration().relayout(Vector2{event.windowSize()} / event.dpiScaling(), event.windowSize(), event.framebufferSize());

    LayerManager::forEachLayer([&](Layer* layer) {
        layer->viewportEvent(event);
    });
}

void GameEngine::keyPressEvent(KeyEvent& event)
{
    // _imGuiLayer->GetIntegration().handleKeyPressEvent(event);
    // if (ImGui::GetIO().WantCaptureKeyboard) return;

    LayerManager::forEachLayerReverse([&](Layer* layer) {
        if (!event.isAccepted())
            layer->keyPressEvent(event);
    });
}

void GameEngine::keyReleaseEvent(KeyEvent& event)
{
    // _imGuiLayer->GetIntegration().handleKeyReleaseEvent(event);
    // if (ImGui::GetIO().WantCaptureKeyboard) return;

    LayerManager::forEachLayerReverse([&](Layer* layer) {
        if (!event.isAccepted())
            layer->keyReleaseEvent(event);
    });
}

void GameEngine::mousePressEvent(MouseEvent& event)
{

    _mouseDownPos = event.position();
    // _imGuiLayer->GetIntegration().handleMousePressEvent(event);
    // if (ImGui::GetIO().WantCaptureMouse) return;

    LayerManager::forEachLayerReverse([&](Layer* layer) {
        if (!event.isAccepted())
            layer->mousePressEvent(event);
    });
}

void GameEngine::mouseReleaseEvent(MouseEvent& event)
{
    // _imGuiLayer->GetIntegration().handleMouseReleaseEvent(event);
    // if (ImGui::GetIO().WantCaptureMouse) return;

    LayerManager::forEachLayerReverse([&](Layer* layer) {
        if (!event.isAccepted())
            layer->mouseReleaseEvent(event);
    });
}

void GameEngine::mouseMoveEvent(MouseMoveEvent& event)
{
    // mousePos = event.position();

    // int count = 0;
    // glfwGetMonitorUserPointer(glfwGetMonitors(&count)[0]);
    // _imGuiLayer->GetIntegration().handleMouseMoveEvent(event);
    // if (ImGui::GetIO().WantCaptureMouse) return;

    LayerManager::forEachLayerReverse([&](Layer* layer) {
        if (!event.isAccepted())
            layer->mouseMoveEvent(event);
    });
}

void GameEngine::mouseScrollEvent(MouseScrollEvent& event)
{
    // _imGuiLayer->GetIntegration().handleMouseScrollEvent(event);
    // if (ImGui::GetIO().WantCaptureMouse) return;

    LayerManager::forEachLayerReverse([&](Layer* layer) {
        if (!event.isAccepted())
            layer->mouseScrollEvent(event);
    });
}

void GameEngine::textInputEvent(TextInputEvent& event)
{
    // _imGuiLayer->GetIntegration().handleTextInputEvent(event);
    // if (ImGui::GetIO().WantCaptureKeyboard) return;

    LayerManager::forEachLayerReverse([&](Layer* layer) {
        if (!event.isAccepted())
            layer->textInputEvent(event);
    });
}

void GameEngine::exitEvent(ExitEvent& event)
{
    event.setAccepted();
}

void GameEngine::drawTitlebar()
{
    auto viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags host_window_flags = 0;
    host_window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
    host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoTitleBar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::PushStyleColor(ImGuiCol_TitleBg, {41.f / 255.f, 74.f / 255.f, 122.f / 255.f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, {41.f / 255.f, 74.f / 255.f, 122.f / 255.f, 1.0f});

    ImGui::Begin("###DockSpaceMainViewport", nullptr, host_window_flags);
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);

    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

    ImGui::End();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N")) {}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Settings", "Ctrl+,")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
            if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

} // namespace Ham

MAGNUM_APPLICATION_MAIN(Ham::GameEngine)