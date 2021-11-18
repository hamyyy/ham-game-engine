#pragma once

#include "Core/Core.hpp"

namespace Ham
{

class Entity;

class Scene
{
public:
    friend class Entity;

    Scene() {}

    ~Scene() {}

    Entity CreateEntity(std::string tag)
    {
        if (tag.rfind("Editor Camera", 0) == 0) // the name "Editor Camera XXX" is resereved for internal use.
        {
            HAM_CORE_ASSERT(false, "Illegal entity name prefix: \"Editor Camera\"");
            return {};
        }

        auto view = registry.view<Component::Tag>();
        for (auto entity : view) // check for already existing entities with that name, if it exists, dont add it to the registry
        {
            if (view.get<Component::Tag>(entity).tag == tag)
            {
                HAM_CORE_WARN("Entity with the name \"{0}\" already exists!, skipping \"CreateEntity()\"", tag);
                return {};
            }
        }

        Entity entity(registry.create(), this);
        entity.AddComponent<Component::Tag>(tag);
        entity.AddComponent<Component::Transform>();
        return entity;
    }

    Entity GetEntity(std::string tag)
    {
        auto view = registry.view<Component::Tag>();
        for (auto entity : view)
        {
            if (view.get<Component::Tag>(entity).tag == tag)
                return {entity, this};
        }

        HAM_CORE_ASSERT(false, std::string("Could not find entity \"") + tag + "\"");
        return {};
    }

    auto RemoveEntity(Entity entity)
    {
        return registry.destroy(entity);
    }

    template <typename C>
    auto GetComponent()
    {
        return registry.view<C>();
    }

    template <typename... Args>
    auto GetComponents()
    {
        return registry.group<Args...>();
    }

    Entity GetEditorCamera(uint32_t index = 0)
    {
        auto view = registry.group<Component::Tag, Component::Index>();
        for (auto entity : view)
        {
            auto [tag, i] = view.get<Component::Tag, Component::Index>(entity);

            if (tag.tag == "Editor Camera" && i.index == index)
                return {entity, this};
        }
        HAM_CORE_WARN("Could not find entity \"Editor Camera\" index: {0}", index);
        return {};
    }

    Entity AddEditorCamera()
    {
        Entity entity(registry.create(), this);
        entity.AddComponent<Component::Tag>("Editor Camera");
        entity.AddComponent<Component::Index>(editorCameraCount);
        entity.AddComponent<Component::Camera>(Magnum::Deg(35.0f), Vector2{16, 9}.aspectRatio(), 0.01f, 100.0f); // perspective camera by default
        entity.AddComponent<Component::Transform>();

        editorCameraCount++;
        return entity;
    }

    void RemoveEditorCamera(Entity cameraEntity)
    {
        if (!cameraEntity.HasComponent<Component::Tag>() || cameraEntity.GetComponent<Component::Tag>().tag != "Editor Camera")
        {
            HAM_CORE_WARN("Entity \"{0:x}\" is not a valid editor camera", (entt::entity)cameraEntity);
            return;
        }
        registry.destroy(cameraEntity);
        editorCameraCount--;
    }

    void RemoveEditorCamera(uint32_t index)
    {
        auto cam = GetEditorCamera(index);
        if (cam)
        {
            registry.destroy(cam);
            editorCameraCount--;
        }
    }

private:
    entt::registry registry;
    uint32_t       editorCameraCount = 0;
};
} // namespace Ham