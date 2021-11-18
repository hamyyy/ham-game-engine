#include "Layers/RenderLayer2.hpp"

#include <imgui_internal.h>

namespace Ham
{

RenderLayer2::RenderLayer2(Scene* scene) :
    _scene(scene),
    Layer("RenderLayer2")
{
    _layer = _scene->CreateEntity(GetName());
}

RenderLayer2::~RenderLayer2() {}

void RenderLayer2::start()
{
    _layer.AddComponent<Component::Gui::GLWindow>("Scene2");

    auto camera = _scene->AddEditorCamera();
    // camera.AddComponent<Component::Camera>(Magnum::Deg(35.0f), Vector2{16, 9}.aspectRatio(), 0.01f, 100.0f);

    camera.GetComponent<Component::Transform>().position.z() = -10.0f;
    camera.GetComponent<Component::Transform>().rotation.y() = 180.0_degf;

    Trade::MeshData cubePrimitive = Primitives::cubeSolid();
    GL::Buffer      vertices;
    GL::Buffer      indices;
    std::pair       compressed = MeshTools::compressIndices(cubePrimitive.indicesAsArray());

    vertices.setData(MeshTools::interleave(cubePrimitive.positions3DAsArray(), cubePrimitive.normalsAsArray()));
    indices.setData(compressed.first);

    auto  cube      = _scene->CreateEntity("Cube2");
    auto& transform = cube.GetComponent<Component::Transform>();
    auto& material  = cube.AddComponent<Component::Material>();
    auto& mesh      = cube.AddComponent<Component::Mesh>();

    transform.position.y() = -1.5f;

    material.color = Color3::fromHsv({60.0_degf, 1.0f, 1.0f});
    // transform.rotation.x() = 30.0f;
    // transform.rotation.y() = 40.0f;

    mesh
        .setPrimitive(cubePrimitive.primitive())
        .setCount(cubePrimitive.indexCount())
        .addVertexBuffer(std::move(vertices), 0, Shaders::PhongGL::Position{}, Shaders::PhongGL::Normal{})
        .setIndexBuffer(std::move(indices), 0, compressed.second);
}

void RenderLayer2::exit()
{
}

void RenderLayer2::update()
{
    auto& frameBuffer = _layer.GetComponent<Component::Gui::GLWindow>().frameBuffer;
    auto& size        = _layer.GetComponent<Component::Gui::GLWindow>().size;

    frameBuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth).bind();

    auto& camera       = _scene->GetEditorCamera(1).GetComponent<Component::Camera>();
    camera.aspectRatio = Vector2{size}.aspectRatio();

    System::Draw(_scene, _scene->GetEditorCamera(1));
}

void RenderLayer2::renderUI()
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isOpen) return;

    auto& title = _layer.GetComponent<Component::Gui::GLWindow>().title;
    if (ImGui::Begin(title.c_str()))
    {
        auto& cube = _scene->GetEntity("Cube2");
        if (cube)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            float  titleBarHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().FramePadding.y * 2.0f;
            ImRect windowRect     = ImRect({ImGui::GetWindowPos().x, ImGui::GetWindowPos().y}, ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + ImGui::GetWindowHeight()));
            ImGuizmo::SetRect(windowRect.Min.x, windowRect.Min.y, windowRect.Max.x - windowRect.Min.x, windowRect.Max.y - windowRect.Min.y);

            auto& cameraEntity    = _scene->GetEditorCamera(1);
            auto& camera          = cameraEntity.GetComponent<Component::Camera>();
            auto& cameraTransform = cameraEntity.GetComponent<Component::Transform>();

            auto& entityTransform = cube.GetComponent<Component::Transform>();

            auto cameraView       = cameraTransform.ToMatrix().inverted();
            auto cameraProjection = camera.getProjectionMatrix();

            float entityTransformMatrix[4 * 4];
            ImGuizmo::RecomposeMatrixFromComponents(entityTransform.position.data(), (Magnum::Float*)entityTransform.rotation.data(), entityTransform.scale.data(), entityTransformMatrix);

            if (ImGuizmo::Manipulate(cameraView.data(),
                                     cameraProjection.data(),
                                     ImGuizmo::OPERATION::TRANSLATE | ImGuizmo::OPERATION::ROTATE | ImGuizmo::OPERATION::SCALE,
                                     ImGuizmo::MODE::WORLD,
                                     entityTransformMatrix) &&
                ImGui::IsWindowFocused())
            {
                ImGuizmo::DecomposeMatrixToComponents(entityTransformMatrix, entityTransform.position.data(), (Magnum::Float*)entityTransform.rotation.data(), entityTransform.scale.data());
            }
        }
    }
    ImGui::End();
}

void RenderLayer2::viewportEvent(Magnum::Platform::Application::ViewportEvent& event) {}
void RenderLayer2::keyPressEvent(Magnum::Platform::Application::KeyEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    event.setAccepted();
}
void RenderLayer2::keyReleaseEvent(Magnum::Platform::Application::KeyEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    event.setAccepted();
}
void RenderLayer2::mousePressEvent(Magnum::Platform::Application::MouseEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    event.setAccepted();
}
void RenderLayer2::mouseReleaseEvent(Magnum::Platform::Application::MouseEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    event.setAccepted();
}
void RenderLayer2::mouseMoveEvent(Magnum::Platform::Application::MouseMoveEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    if (!(event.buttons() & Magnum::Platform::Application::MouseMoveEvent::Button::Left)) return;

    auto& size = _layer.GetComponent<Component::Gui::GLWindow>().size;

    Vector2 delta = 3.0f * Vector2{event.relativePosition()} / Vector2{size};

    auto& cube = _scene->GetEntity("Cube2").GetComponent<Component::Transform>();

    // cube.rotation.x() += delta.y();
    // cube.rotation.y() += delta.x();

    event.setAccepted();
}
void RenderLayer2::mouseScrollEvent(Magnum::Platform::Application::MouseScrollEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    event.setAccepted();
}
void RenderLayer2::textInputEvent(Magnum::Platform::Application::TextInputEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    event.setAccepted();
}

} // namespace Ham