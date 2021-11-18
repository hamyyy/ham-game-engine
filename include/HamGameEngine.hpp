#pragma once

#include "Core/Core.hpp"

#include "Layers/ImGuiLayer.hpp"
#include "Layers/RenderLayer.hpp"
#include "Layers/RenderLayer2.hpp"

using namespace Math::Literals;

namespace Ham
{
// using namespace Magnum::ImGuiIntegration;

class GameEngine : public Platform::Application
{
public:
    explicit GameEngine(const Arguments& arguments);

    // event handlers
    void viewportEvent(ViewportEvent& event) override;
    void keyPressEvent(KeyEvent& event) override;
    void keyReleaseEvent(KeyEvent& event) override;
    void mousePressEvent(MouseEvent& event) override;
    void mouseReleaseEvent(MouseEvent& event) override;
    void mouseMoveEvent(MouseMoveEvent& event) override;
    void mouseScrollEvent(MouseScrollEvent& event) override;
    void textInputEvent(TextInputEvent& event) override;
    void exitEvent(ExitEvent& event) override;


private:
    void drawEvent() override;
    void drawTitlebar();

    Scene _scene;

    ImGuiLayer* _imGuiLayer;

    bool   _showDemoWindow    = true;
    bool   _showAnotherWindow = false;
    Color4 _clearColor        = 0x72909aff_rgbaf;
    Float  _floatValue        = 0.0f;

    Vector2i _mouseDownPos;

    GL::Mesh         _mesh;
    Shaders::PhongGL _shader;

    Matrix4 _transformation, _projection;
    Color3  _color;
};

} // namespace Ham
