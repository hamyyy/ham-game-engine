#include "HamGameEngine.hpp"

using WindowFlag = Magnum::Platform::Application::Configuration::WindowFlag;

namespace Ham
{


GameEngine::GameEngine(const Arguments& arguments) :
    Platform::Application{arguments,
                          Configuration{}
                              .setTitle("Magnum Primitives Example")
                              .addWindowFlags(WindowFlag::Resizable)
                              .setSize(Magnum::Vector2i{1280, 720})},
    _frameBuffer({{}, {500, 500}})
{
    Log::Initialize();
    LayerManager::Initialize();


    _imGuiLayer                   = new ImGuiLayer(&_scene);
    _imGuiLayer->GetIntegration() = ImGuiIntegration::Context(Vector2{windowSize()} / dpiScaling(), windowSize(), framebufferSize());

    LayerManager::PushOverlay(_imGuiLayer);
    LayerManager::PushLayer(new RenderLayer(&_scene));
    
    _frameBufferColor.setStorage(1, GL::TextureFormat::RGBA8, _frameBuffer.viewport().size());
    _frameBufferDepthStencil.setStorage(GL::RenderbufferFormat::DepthComponent24, _frameBuffer.viewport().size());

    _frameBuffer
        .attachTexture(GL::Framebuffer::ColorAttachment{0}, _frameBufferColor, 1)
        .attachRenderbuffer(GL::Framebuffer::BufferAttachment::DepthStencil, _frameBufferDepthStencil);
}

void GameEngine::drawEvent()
{
    // _frameBuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth).bind();
    _frameBuffer.clearColor(0, _clearColor)
        .clearDepth(1.0f)
        .bind();

    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth).bind();

    if (ImGui::GetIO().WantTextInput && !isTextInputActive())
        startTextInput();
    else if (!ImGui::GetIO().WantTextInput && isTextInputActive())
        stopTextInput();

    _imGuiLayer->begin();


    _frameBuffer.bind();

    // _frameBuffer.setViewport({{}, size});
    // _frameBufferColor.setStorage(1, GL::TextureFormat::RGBA8, size);
    // _frameBufferDepthStencil.setStorage(GL::RenderbufferFormat::DepthComponent24, size);

    LayerManager::UpdateLayers();
    GL::defaultFramebuffer.bind();

    GL::Renderer::enable(GL::Renderer::Feature::Blending);
    GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);
    GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);
    GL::Renderer::disable(GL::Renderer::Feature::DepthTest);

    _imGuiLayer->GetIntegration().updateApplicationCursor(*this);
    _imGuiLayer->renderUI(_frameBufferColor);
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

    _imGuiLayer->GetIntegration().relayout(Vector2{event.windowSize()} / event.dpiScaling(), event.windowSize(), event.framebufferSize());
}

void GameEngine::keyPressEvent(KeyEvent& event)
{
    if (_imGuiLayer->GetIntegration().handleKeyPressEvent(event)) return;
}

void GameEngine::keyReleaseEvent(KeyEvent& event)
{
    if (_imGuiLayer->GetIntegration().handleKeyReleaseEvent(event)) return;
}

void GameEngine::mousePressEvent(MouseEvent& event)
{
    if (_imGuiLayer->GetIntegration().handleMousePressEvent(event)) return;
    if (event.button() != MouseEvent::Button::Left) return;

    event.setAccepted();
}

void GameEngine::mouseReleaseEvent(MouseEvent& event)
{
    if (_imGuiLayer->GetIntegration().handleMouseReleaseEvent(event)) return;

    event.setAccepted();
    redraw();
}

void GameEngine::mouseMoveEvent(MouseMoveEvent& event)
{
    if (_imGuiLayer->GetIntegration().handleMouseMoveEvent(event)) return;
    if (!(event.buttons() & MouseMoveEvent::Button::Left)) return;

    Vector2 delta = 3.0f * Vector2{event.relativePosition()} / Vector2{windowSize()};

    auto& camera = _scene.GetEntity("Main Camera").GetComponent<Component::Transform>();

    camera.rotation.x() += delta.y();
    camera.rotation.y() += delta.x();

    // _transformation = Matrix4::rotationX(Rad{delta.y()}) * _transformation * Matrix4::rotationY(Rad{delta.x()});

    event.setAccepted();
    redraw();
}

void GameEngine::mouseScrollEvent(MouseScrollEvent& event)
{
    if (_imGuiLayer->GetIntegration().handleMouseScrollEvent(event))
    {
        /* Prevent scrolling the page */
        event.setAccepted();
        return;
    }
}

void GameEngine::textInputEvent(TextInputEvent& event)
{
    if (_imGuiLayer->GetIntegration().handleTextInputEvent(event)) return;
}

void GameEngine::exitEvent(ExitEvent& event)
{
    event.setAccepted();
}

} // namespace Ham

MAGNUM_APPLICATION_MAIN(Ham::GameEngine)