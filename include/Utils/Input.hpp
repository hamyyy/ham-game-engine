#pragma once

namespace Ham
{
using Key    = Platform::Application::KeyEvent::Key;
using Button = Platform::Application::MouseEvent::Button;

using KeyRegistry         = std::map<Key, bool>;
using MouseButtonRegistry = std::map<Button, bool>;

class Input
{
public:
    inline static bool IsKeyPressed(Platform::Application::KeyEvent::Key key)
    {
        if (_keyRegistry.count(key) == 0)
            return false;
        return _keyRegistry.at(key);
    }

    inline static void SetKeyPressed(Platform::Application::KeyEvent::Key key, bool value)
    {
        _keyRegistry.insert_or_assign(key, value);
    }

    inline static bool IsButtonPressed(Platform::Application::MouseEvent::Button button)
    {
        if (_mouseButtonRegistry.count(button) == 0)
            return false;

        return _mouseButtonRegistry.at(button);
    }

    inline static void SetButtonPressed(Platform::Application::MouseEvent::Button button, bool value)
    {
        _mouseButtonRegistry.insert_or_assign(button, value);
    }

    inline static void SetMousePosition(const Vector2i& position)
    {
        _mousePosition = position;
    }

    inline static Vector2i GetMousePosition()
    {
        return _mousePosition;
    }

private:
    inline static KeyRegistry         _keyRegistry;
    inline static MouseButtonRegistry _mouseButtonRegistry;
    inline static Vector2i            _mousePosition;
};
} // namespace Ham
