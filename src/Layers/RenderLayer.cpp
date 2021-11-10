#include "Layers/RenderLayer.hpp"

namespace Ham
{
void RenderLayer::start()
{
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

    GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add, GL::Renderer::BlendEquation::Add);
    GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::SourceAlpha, GL::Renderer::BlendFunction::OneMinusSourceAlpha);

    auto camera = _scene->CreateEntity("Main Camera");
    camera.AddComponent<Component::Camera>(35.0_degf, Vector2{16, 9}.aspectRatio(), 0.01f, 100.0f);
    camera.GetComponent<Component::Transform>().position.z() = -10.0f;

    Trade::MeshData cubePrimitive = Primitives::cubeSolid();
    GL::Buffer      vertices;
    GL::Buffer      indices;
    std::pair       compressed = MeshTools::compressIndices(cubePrimitive.indicesAsArray());

    vertices.setData(MeshTools::interleave(cubePrimitive.positions3DAsArray(), cubePrimitive.normalsAsArray()));
    indices.setData(compressed.first);

    auto  cube      = _scene->CreateEntity("Cube");
    auto& material  = cube.AddComponent<Component::Material>();
    auto& mesh      = cube.AddComponent<Component::Mesh>();
    auto& transform = cube.GetComponent<Component::Transform>();

    material.color = Color3::fromHsv({35.0_degf, 1.0f, 1.0f});
    // transform.rotation.x() = 30.0f;
    // transform.rotation.y() = 40.0f;

    mesh
        .setPrimitive(cubePrimitive.primitive())
        .setCount(cubePrimitive.indexCount())
        .addVertexBuffer(std::move(vertices), 0, Shaders::PhongGL::Position{}, Shaders::PhongGL::Normal{})
        .setIndexBuffer(std::move(indices), 0, compressed.second);
}

void RenderLayer::exit()
{
}

void RenderLayer::update()
{
    System::Draw(_scene);
}
} // namespace Ham