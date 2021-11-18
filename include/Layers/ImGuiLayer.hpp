#pragma once

#include "Core/Core.hpp"

namespace Ham
{
class ImGuiLayer : public Layer
{
public:
    ImGuiLayer(Scene* scene);
    ImGuiLayer(Scene* scene, const Vector2& size, const Vector2i& windowSize, const Vector2i& framebufferSize);

    ~ImGuiLayer();

    void start() override;
    void exit() override;
    void update() override{};

    void renderUI() override;

    void begin();
    void end();

    void viewportEvent(Magnum::Platform::Application::ViewportEvent& event) override;
    void keyPressEvent(Magnum::Platform::Application::KeyEvent& event) override;
    void keyReleaseEvent(Magnum::Platform::Application::KeyEvent& event) override;
    void mousePressEvent(Magnum::Platform::Application::MouseEvent& event) override;
    void mouseReleaseEvent(Magnum::Platform::Application::MouseEvent& event) override;
    void mouseMoveEvent(Magnum::Platform::Application::MouseMoveEvent& event) override;
    void mouseScrollEvent(Magnum::Platform::Application::MouseScrollEvent& event) override;
    void textInputEvent(Magnum::Platform::Application::TextInputEvent& event) override;

    inline ImGuiIntegration::Context& GetIntegration() { return _integration; }

private:
    ImGuiIntegration::Context _integration{NoCreate};
    Scene*                    _scene;
    bool _sceneIsFocused = false;
    bool _isTitlebarBeingDragged = false;
    Vector2i _mouseDownPos;
};
} // namespace Ham