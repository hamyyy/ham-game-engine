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

    void renderUI() override;

    void begin();
    void end();

    void viewportEvent(Platform::Application::ViewportEvent& event) override;
    void keyPressEvent(Platform::Application::KeyEvent& event) override;
    void keyReleaseEvent(Platform::Application::KeyEvent& event) override;
    void mousePressEvent(Platform::Application::MouseEvent& event) override;
    void mouseReleaseEvent(Platform::Application::MouseEvent& event) override;
    void mouseMoveEvent(Platform::Application::MouseMoveEvent& event) override;
    void mouseScrollEvent(Platform::Application::MouseScrollEvent& event) override;
    void textInputEvent(Platform::Application::TextInputEvent& event) override;

    inline ImGuiIntegration::Context& GetIntegration() { return _integration; }

private:
    ImGuiIntegration::Context _integration{NoCreate};
    Scene*                    _scene;
    bool _sceneIsFocused = false;
    bool _isTitlebarBeingDragged = false;
    Vector2i _mouseDownPos;
};
} // namespace Ham