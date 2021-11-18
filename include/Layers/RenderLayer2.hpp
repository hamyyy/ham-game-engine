#pragma once

#include "Core/Core.hpp"

namespace Ham
{
class RenderLayer2 : public Layer
{
public:
    RenderLayer2(Scene* scene);
    ~RenderLayer2();

    void start() override;
    void exit() override;
    void update() override;

    void renderUI() override;

    void viewportEvent(Magnum::Platform::Application::ViewportEvent& event) override;
    void keyPressEvent(Magnum::Platform::Application::KeyEvent& event) override;
    void keyReleaseEvent(Magnum::Platform::Application::KeyEvent& event) override;
    void mousePressEvent(Magnum::Platform::Application::MouseEvent& event) override;
    void mouseReleaseEvent(Magnum::Platform::Application::MouseEvent& event) override;
    void mouseMoveEvent(Magnum::Platform::Application::MouseMoveEvent& event) override;
    void mouseScrollEvent(Magnum::Platform::Application::MouseScrollEvent& event) override;
    void textInputEvent(Magnum::Platform::Application::TextInputEvent& event) override;

private:
    Scene* _scene;
    Entity _layer;
};
} // namespace Ham