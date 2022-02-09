#include "Layers/EditorLayer.hpp"

#include <imgui_internal.h>

namespace Ham
{

EditorLayer::EditorLayer(Scene* scene) :
    _scene(scene),
    Layer("EditorLayer")
{
    _layer = _scene->CreateEntity(GetName());
}

EditorLayer::~EditorLayer() {}

void EditorLayer::start()
{

    auto camera = _scene->AddEditorCamera();
    Debug::AddCameraFrustum(camera);
    _layer.AddComponent<Component::Gui::GLWindow>("Scene2", camera);
    // camera.AddComponent<Component::Camera>(Deg(35.0f), Vector2{16, 9}.aspectRatio(), 0.01f, 100.0f);

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
    auto& material  = cube.AddComponent<Component::ShaderMaterial>();
    auto& mesh      = cube.AddComponent<Component::Mesh>();

    transform.position.y() = -1.5f;

    auto color = Color3::fromHsv({60.0_degf, 1.0f, 1.0f});

    material.color = color;
    // material.shader
    //     .setDiffuseColor(color)
    //     .setAmbientColor(Color3::fromHsv({color.hue(), color.saturation(), color.value() * 0.5f}));

    // transform.rotation.x() = 30.0f;
    // transform.rotation.y() = 40.0f;

    mesh.mesh
        .setPrimitive(cubePrimitive.primitive())
        .setCount(cubePrimitive.indexCount())
        .addVertexBuffer(std::move(vertices), 0, Shaders::PhongGL::Position{}, Shaders::PhongGL::Normal{})
        .setIndexBuffer(std::move(indices), 0, compressed.second);
}

void EditorLayer::exit()
{
}

void EditorLayer::update()
{
    auto& frameBuffer = _layer.GetComponent<Component::Gui::GLWindow>().frameBuffer;
    auto& size        = _layer.GetComponent<Component::Gui::GLWindow>().size;

    frameBuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth).bind();

    auto& camera       = _scene->GetEditorCamera(1).GetComponent<Component::Camera>();
    camera.aspectRatio = Vector2{size}.aspectRatio();

    EditorCamera::HandleCameraMovement(_layer);

    System::Draw(_scene, _scene->GetEditorCamera(1));
    System::DebugDraw(_scene, _scene->GetEditorCamera(1));
    Debug::UpdateCameraFrustum(_scene->GetEditorCamera(1));
}

void EditorLayer::renderUI()
{
    // if (!_layer.GetComponent<Component::Gui::GLWindow>().isOpen) return;

    ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Properties"))
    {
        Entity selectedEntity = _scene->GetSelectedEntity();

        if (selectedEntity)
        {
            GUI::DrawTransform(selectedEntity);
        }
    }
    ImGui::End();

    ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Hierarchy"))
    {
        GUI::DrawSceneHierarchy(_scene);
    }
    ImGui::End();

    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_NoDockingSplitMe | ImGuiDockNodeFlags_NoDockingSplitOther | ImGuiDockNodeFlags_NoResizeY;
    ImGui::SetNextWindowClass(&window_class);

    float toolBarHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().FramePadding.y * 4.0f;

    ImGui::SetNextWindowSize(ImVec2(ImGui::GetMainViewport()->Size.x, toolBarHeight));
    // ImGui::SetNextWindowSize(ImVec2(ImGui::GetMainViewport()->Size.x, toolBarHeight), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Toolbar"))
    {
        if (ImGui::Button("T"))
        {
            Settings::manipulateOperation = ImGuizmo::OPERATION::TRANSLATE;
        }

        ImGui::SameLine();
        if (ImGui::Button("R"))
        {
            Settings::manipulateOperation = ImGuizmo::OPERATION::ROTATE;
        }

        ImGui::SameLine();
        if (ImGui::Button("S"))
        {
            Settings::manipulateOperation = ImGuizmo::OPERATION::SCALE;
        }
    }
    ImGui::End();
}

void EditorLayer::viewportEvent(Platform::Application::ViewportEvent& event) {}
void EditorLayer::keyPressEvent(Platform::Application::KeyEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    event.setAccepted();
}
void EditorLayer::keyReleaseEvent(Platform::Application::KeyEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    event.setAccepted();
}
void EditorLayer::mousePressEvent(Platform::Application::MouseEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    event.setAccepted();
}
void EditorLayer::mouseReleaseEvent(Platform::Application::MouseEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    event.setAccepted();
}
void EditorLayer::mouseMoveEvent(Platform::Application::MouseMoveEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    if (!(event.buttons() & Platform::Application::MouseMoveEvent::Button::Left)) return;

    auto& size = _layer.GetComponent<Component::Gui::GLWindow>().size;

    Vector2 delta = 3.0f * Vector2{event.relativePosition()} / Vector2{size};

    auto& cube = _scene->GetEntity("Cube2").GetComponent<Component::Transform>();

    // cube.rotation.x() += delta.y();
    // cube.rotation.y() += delta.x();

    event.setAccepted();
}
void EditorLayer::mouseScrollEvent(Platform::Application::MouseScrollEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    event.setAccepted();
}
void EditorLayer::textInputEvent(Platform::Application::TextInputEvent& event)
{
    if (!_layer.GetComponent<Component::Gui::GLWindow>().isFocused) return;
    event.setAccepted();
}

} // namespace Ham