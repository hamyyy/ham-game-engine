#pragma once

namespace Ham
{
class Layer
{
public:
    Layer(const std::string& name = "Layer") :
        _name(name){};
    virtual ~Layer() {}

    virtual void start() {}
    virtual void update() {}
    virtual void renderUI(){};
    virtual void exit() {}

    virtual void viewportEvent(Magnum::Platform::Application::ViewportEvent& event) {}
    virtual void keyPressEvent(Magnum::Platform::Application::KeyEvent& event) {}
    virtual void keyReleaseEvent(Magnum::Platform::Application::KeyEvent& event) {}
    virtual void mousePressEvent(Magnum::Platform::Application::MouseEvent& event) {}
    virtual void mouseReleaseEvent(Magnum::Platform::Application::MouseEvent& event) {}
    virtual void mouseMoveEvent(Magnum::Platform::Application::MouseMoveEvent& event) {}
    virtual void mouseScrollEvent(Magnum::Platform::Application::MouseScrollEvent& event) {}
    virtual void textInputEvent(Magnum::Platform::Application::TextInputEvent& event) {}

    std::string GetName() { return _name; }
    bool        isVisible() { return _visible; }

protected:
    bool _visible = true;

private:
    std::string _name;
};
} // namespace Ham