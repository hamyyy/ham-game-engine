#pragma once

#include "Core/Core.hpp"
#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Scene.hpp"

using namespace Math::Literals;

namespace Ham::System
{
namespace Helper
{
void PrintMatrix4(Magnum::Matrix4 mat)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << mat[i][j] << ", ";
        }
        std::cout << "\n";
    }
}
} // namespace Helper

void Print(Scene* scene)
{
    // for (auto& [entity, pos] : scene->GetComponent<Component::Transform>().each())
    // {
    //     std::cout << "position: (" << pos.x << ", " << pos.y << ")" << std::endl;
    // }

    auto view = scene->GetComponent<Component::Transform>();
    for (auto entity : view)
    {
        auto& transform = view.get<Component::Transform>(entity);

        std::cout << "position:" << std::endl;
        Helper::PrintMatrix4(transform.ToMatrix());
        std::cout << std::endl;
    }
}

void Move(Scene* scene)
{
    // for (auto& [entity, pos] : scene->GetComponent<Component::Position>().each())
    // {
    //     pos.x += 0.1f;
    //     pos.y += 0.1f;
    // }

    auto view = scene->GetComponent<Component::Transform>();
    for (auto entity : view)
    {
        auto& transform = view.get<Component::Transform>(entity);

        transform.scale *= {1.0001f, 1.0001f, 1.0001f};
    }
}

void Draw(Scene* scene)
{
    auto camera           = scene->GetEntity("Main Camera");
    auto cameraTransform = camera.GetComponent<Component::Transform>();
    cameraTransform.scale = {1.0f, 1.0f, 1.0f};
    auto projectionMatrix = camera.GetComponent<Component::Camera>().getProjectionMatrix() * cameraTransform.ToMatrix();

    auto view = scene->GetComponents<Component::Transform, Component::Material, Component::Mesh>();
    for (auto entity : view)
    {
        Component::Transform& transform = view.get<Component::Transform>(entity);
        Component::Material&  material  = view.get<Component::Material>(entity);
        Component::Mesh&      mesh      = view.get<Component::Mesh>(entity);

        // Magnum::GL::Mesh mesh;

        // mesh
        //     .setCount(meshComponent.indexCount)
        //     .addVertexBuffer(std::move(meshComponent.vertexBuffer), 0, Shaders::PhongGL::Position{}, Shaders::PhongGL::Normal{})
        //     .setIndexBuffer(std::move(meshComponent.indexBuffer), 0, meshComponent.indexType);
        material
            .setLightPositions({{1.4f, 1.0f, 0.75f, 0.0f}})
            .setDiffuseColor(material.color)
            .setAmbientColor(Magnum::Color3::fromHsv({material.color.hue(), material.color.saturation(), material.color.value() * 0.5f}))
            .setTransformationMatrix(transform.ToMatrix())
            .setNormalMatrix(transform.ToMatrix().normalMatrix())
            .setProjectionMatrix(projectionMatrix)
            .draw(mesh);
    }
}
} // namespace Ham::System
