#pragma once

#include "Core/Core.hpp"

namespace Ham
{

class Scene;

class Entity
{
public:
    Entity() = default;
    Entity(entt::entity s_entity_handle, Scene* s_scene) :
        entity_handle(s_entity_handle), scene(s_scene){};

    ~Entity() = default;

    template <typename T, typename... Args>
    T& AddComponent(Args&&... args)
    {
        if (HasComponent<T>())
            return GetComponent<T>();

        return scene->registry.emplace<T>(entity_handle, std::forward<Args>(args)...);
    }

    template <typename T>
    T& GetComponent()
    {
        if (!HasComponent<T>())
            HAM_CORE_ASSERT(false, "Entity does not have component");

        return scene->registry.get<T>(entity_handle);
    }

    template <typename... Args>
    auto& GetComponents()
    {
        return scene->registry.get<Args...>(entity_handle);
    }

    template <typename T>
    void RemoveComponent()
    {
        scene->registry.remove<T>(entity_handle);
    }

    uint32_t GetId()
    {
        return (uint32_t)entity_handle;
    }

    template <typename T>
    bool HasComponent()
    {
        return scene->registry.try_get<T>(entity_handle) != nullptr;
    }

    operator entt::entity&()
    {
        return entity_handle;
    }

    operator bool() const { return entity_handle != entt::null; }

private:
    Scene*       scene = nullptr;
    entt::entity entity_handle{entt::null};
};
} // namespace Ham