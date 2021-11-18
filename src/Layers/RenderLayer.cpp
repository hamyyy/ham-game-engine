#include "Layers/RenderLayer.hpp"

#include <imgui_internal.h>

namespace Ham
{

RenderLayer::RenderLayer(Scene* scene) :
    _scene(scene),
    Layer("RenderLayer")
{
    _layer = _scene->CreateEntity(GetName());
}

RenderLayer::~RenderLayer() {}

void RenderLayer::start()
{
    _layer.AddComponent<Component::Gui::GLWindow>("Scene");

    auto camera = _scene->AddEditorCamera();
    // camera.AddComponent<Component::Camera>(Magnum::Deg(35.0f), Vector2{16, 9}.aspectRatio(), 0.01f, 100.0f);

    camera.GetComponent<Component::Transform>().position.z() = 10.0f;

    Trade::MeshData cubePrimitive = Primitives::cubeSolid();
    GL::Buffer      vertices;
    GL::Buffer      indices;
    std::pair       compressed = MeshTools::compressIndices(cubePrimitive.indicesAsArray());

    vertices.setData(MeshTools::interleave(cubePrimitive.positions3DAsArray(), cubePrimitive.normalsAsArray()));
    indices.setData(compressed.first);

    // auto  cube      = _scene->CreateEntity("Cube");
    // auto& material  = cube.AddComponent<Component::Material>();
    // auto& mesh      = cube.AddComponent<Component::Mesh>();
    // auto& transform = cube.GetComponent<Component::Transform>();

    // material.color = Color3::fromHsv({35.0_degf, 1.0f, 1.0f});
    // transform.rotation.x() = 30.0f;
    // transform.rotation.y() = 40.0f;

    // mesh
    //     .setPrimitive(cubePrimitive.primitive())
    //     .setCount(cubePrimitive.indexCount())
    //     .addVertexBuffer(std::move(vertices), 0, Shaders::PhongGL::Position{}, Shaders::PhongGL::Normal{})
    //     .setIndexBuffer(std::move(indices), 0, compressed.second);
}

void RenderLayer::exit()
{
}

void RenderLayer::update()
{
    auto& frameBuffer = _layer.GetComponent<Component::Gui::GLWindow>().frameBuffer;
    auto& size        = _layer.GetComponent<Component::Gui::GLWindow>().size;

    frameBuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth).bind();

    auto& camera       = _scene->GetEditorCamera(0).GetComponent<Component::Camera>();
    camera.aspectRatio = Vector2{size}.aspectRatio();

    System::Draw(_scene, _scene->GetEditorCamera(0));
}

void RenderLayer::renderUI()
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isOpen) return;

    auto& title = _layer.GetComponent<Component::Gui::GLWindow>().title;
    if (ImGui::Begin(title.c_str()))
    {
        // auto& cube = _scene->GetEntity("Cube");
        // if (cube)
        // {
        //     ImGuizmo::SetOrthographic(false);
        //     ImGuizmo::SetDrawlist();
        //     float  titleBarHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().FramePadding.y * 2.0f;
        //     ImRect windowRect     = ImRect({ImGui::GetWindowPos().x, ImGui::GetWindowPos().y}, ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + ImGui::GetWindowHeight()));
        //     ImGuizmo::SetRect(windowRect.Min.x, windowRect.Min.y, windowRect.Max.x - windowRect.Min.x, windowRect.Max.y - windowRect.Min.y);

        //     auto& cameraEntity    = _scene->GetEntity("Main Camera");
        //     auto& camera          = cameraEntity.GetComponent<Component::Camera>();
        //     auto& cameraTransform = cameraEntity.GetComponent<Component::Transform>();

        //     auto cameraView       = cameraTransform.ToMatrix().inverted();
        //     auto cameraProjection = camera.getProjectionMatrix();


        //     auto entityTransform = cube.GetComponent<Component::Transform>().ToMatrix();
        //     // ImGuizmo::DrawCubes(cameraView.data(), cameraProjection.data(), entityTransform.data(), 1);

        //     ImGuizmo::Manipulate(cameraView.data(), cameraProjection.data(), ImGuizmo::OPERATION::TRANSLATE | ImGuizmo::OPERATION::ROTATE | ImGuizmo::OPERATION::SCALE, ImGuizmo::MODE::WORLD, entityTransform.data());
        //     if (ImGuizmo::IsUsing())
        //     {
        //         auto& t        = cube.GetComponent<Component::Transform>();
        //         t.position     = entityTransform.translation();
        //         auto euler     = Math::Quaternion<Magnum::Float>::fromMatrix(entityTransform.rotation()).toEuler();
        //         t.rotation.x() = ((Magnum::Float)euler.x());
        //         t.rotation.y() = ((Magnum::Float)euler.y());
        //         t.rotation.z() = ((Magnum::Float)euler.z());
        //         t.scale        = entityTransform.scaling();
        //     }
        // }
    }
    ImGui::End();
}

void RenderLayer::viewportEvent(Magnum::Platform::Application::ViewportEvent& event) {}
void RenderLayer::keyPressEvent(Magnum::Platform::Application::KeyEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    event.setAccepted();
}
void RenderLayer::keyReleaseEvent(Magnum::Platform::Application::KeyEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    event.setAccepted();
}
void RenderLayer::mousePressEvent(Magnum::Platform::Application::MouseEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    event.setAccepted();
}
void RenderLayer::mouseReleaseEvent(Magnum::Platform::Application::MouseEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    event.setAccepted();
}
void RenderLayer::mouseMoveEvent(Magnum::Platform::Application::MouseMoveEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    if (!(event.buttons() & Magnum::Platform::Application::MouseMoveEvent::Button::Left)) return;

    auto& size = _layer.GetComponent<Component::Gui::GLWindow>().size;

    Vector2 delta = 3.0f * Vector2{event.relativePosition()} / Vector2{size};

    auto& camera = _scene->GetEditorCamera(0).GetComponent<Component::Transform>();

    // camera.rotation.x() += delta.y();
    // camera.rotation.y() += delta.x();

    event.setAccepted();
}
void RenderLayer::mouseScrollEvent(Magnum::Platform::Application::MouseScrollEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    event.setAccepted();
}
void RenderLayer::textInputEvent(Magnum::Platform::Application::TextInputEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    event.setAccepted();
}

} // namespace Ham