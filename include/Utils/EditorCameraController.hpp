#pragma once

namespace Ham
{

namespace EditorCamera
{
static inline void HandleCameraMovement(Entity GLWindow)
{
    auto& GLWindowComponent = GLWindow.GetComponent<Component::Gui::GLWindow>();
    auto  camera            = GLWindowComponent.camera;
    auto& cameraComponent   = camera.GetComponent<Component::Camera>();
    auto& transform         = camera.GetComponent<Component::Transform>();

    float speed = 10;

    if (Input::IsKeyPressed(Key::W))
    {
        transform.position += transform.GetForward() * speed * Time::DeltaTime;
    }
    if (Input::IsKeyPressed(Key::S))
    {
        transform.position -= transform.GetForward() * speed * Time::DeltaTime;
    }
    if (Input::IsKeyPressed(Key::A))
    {
        transform.position -= transform.GetRight() * speed * Time::DeltaTime;
    }
    if (Input::IsKeyPressed(Key::D))
    {
        transform.position += transform.GetRight() * speed * Time::DeltaTime;
    }
    if (Input::IsKeyPressed(Key::Q))
    {
        transform.position -= transform.GetUp() * speed * Time::DeltaTime;
    }
    if (Input::IsKeyPressed(Key::E))
    {
        transform.position += transform.GetUp() * speed * Time::DeltaTime;
    }
}
} // namespace EditorCamera


} // namespace Ham
