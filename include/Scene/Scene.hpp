#pragma once

#include "Core/Core.hpp"
#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"

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

        HAM_CORE_WARN("Could not find entity, \"" + tag + "\"");
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

private:
    entt::registry registry;
};
} // namespace Ham