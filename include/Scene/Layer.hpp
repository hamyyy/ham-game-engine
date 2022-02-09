#pragma once

namespace Ham
{

class Layer: public Entity
{
public:
    Layer(const std::string& name = "Layer") :
        _name(name){};
    virtual ~Layer() {}

    virtual void start() {}
    virtual void update() {}
    virtual void renderUI(){};
    virtual void exit() {}

    virtual void viewportEvent(Platform::Application::ViewportEvent& event) {}
    virtual void keyPressEvent(Platform::Application::KeyEvent& event) {}
    virtual void keyReleaseEvent(Platform::Application::KeyEvent& event) {}
    virtual void mousePressEvent(Platform::Application::MouseEvent& event) {}
    virtual void mouseReleaseEvent(Platform::Application::MouseEvent& event) {}
    virtual void mouseMoveEvent(Platform::Application::MouseMoveEvent& event) {}
    virtual void mouseScrollEvent(Platform::Application::MouseScrollEvent& event) {}
    virtual void textInputEvent(Platform::Application::TextInputEvent& event) {}

    std::string GetName() { return _name; }
    bool        isVisible() { return _visible; }

protected:
    bool      _visible = true;

private:
    std::string _name;
};
} // namespace Ham