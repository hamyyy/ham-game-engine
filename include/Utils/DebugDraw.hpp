#pragma once

namespace Ham
{
namespace Debug
{

inline static void AddCameraFrustum(Entity camera)
{
    auto& cameraComponent = camera.GetComponent<Component::Camera>();
    auto& material        = camera.AddComponent<Component::MeshVisualizerShaderMaterial>();
    auto& mesh            = camera.AddComponent<Component::DebugMesh>();

    material.color          = {0.0f, 1.0f, 0.0f, 1.0f};
    material.wireframeColor = {1.0f, 1.0f, 1.0f, 1.0f};

    auto cubePrimitive = Primitives::cubeWireframe();

    auto data = cubePrimitive.positions3DAsArray();

    for (auto& point : data)
    {
        point = cameraComponent.getProjectionMatrix().inverted().transformPoint(point);
    }


    GL::Buffer vertices;
    GL::Buffer indices;
    std::pair  compressed = MeshTools::compressIndices(cubePrimitive.indicesAsArray());

    vertices.setData(data, GL::BufferUsage::DynamicDraw);
    indices.setData(compressed.first);

    mesh.mesh
        .setPrimitive(cubePrimitive.primitive())
        .setCount(cubePrimitive.indexCount())
        .addVertexBuffer(std::move(vertices), 0, Shaders::MeshVisualizer3D::Position{})
        .setIndexBuffer(std::move(indices), 0, compressed.second);
}

inline static void UpdateCameraFrustum(Entity camera)
{
    auto& cameraComponent = camera.GetComponent<Component::Camera>();
    auto& mesh            = camera.AddComponent<Component::DebugMesh>();

    auto cubePrimitive = Primitives::cubeWireframe();

    auto data = cubePrimitive.positions3DAsArray();

    for (auto& point : data)
    {
        point = cameraComponent.getProjectionMatrix().inverted().transformPoint(point);
    }

    GL::Buffer vertices;

    vertices.setData(data, GL::BufferUsage::DynamicDraw);

    mesh.mesh.addVertexBuffer(std::move(vertices), 0, Shaders::MeshVisualizer3D::Position{});
}






} // namespace Debug

} // namespace Ham
