#pragma once

#include "Core/Core.hpp"
#include "Scene/Layer.hpp"
#include "Scene/Scene.hpp"

namespace Ham
{
class RenderLayer : public Layer
{
public:
    RenderLayer(Scene* scene) :
        _scene(scene),
        Layer("RenderLayer"){};
    ~RenderLayer() = default;

    void start() override;
    void exit() override;
    void update() override;

private:
    Scene* _scene;
};
} // namespace Ham