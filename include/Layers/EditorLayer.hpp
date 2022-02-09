#pragma once

#include "Core/Core.hpp"

namespace Ham
{
class EditorLayer : public Layer
{
public:
    EditorLayer(Scene* scene);
    ~EditorLayer();

    void start() override;
    void exit() override;
    void update() override;

    void renderUI() override;

    void viewportEvent(Platform::Application::ViewportEvent& event) override;
    void keyPressEvent(Platform::Application::KeyEvent& event) override;
    void keyReleaseEvent(Platform::Application::KeyEvent& event) override;
    void mousePressEvent(Platform::Application::MouseEvent& event) override;
    void mouseReleaseEvent(Platform::Application::MouseEvent& event) override;
    void mouseMoveEvent(Platform::Application::MouseMoveEvent& event) override;
    void mouseScrollEvent(Platform::Application::MouseScrollEvent& event) override;
    void textInputEvent(Platform::Application::TextInputEvent& event) override;

private:
    Scene* _scene;
    Entity _layer;
};
} // namespace Ham