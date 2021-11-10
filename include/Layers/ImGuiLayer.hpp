#pragma once

#include "Core/Core.hpp"
#include "Scene/Layer.hpp"
#include "Scene/Scene.hpp"

namespace Ham
{
class ImGuiLayer : public Layer
{
public:
    ImGuiLayer(Scene* scene) :
        _scene(scene),
        Layer("ImGuiLayer"){};
    ~ImGuiLayer() = default;

    void start() override;
    void exit() override;
    void update() override{};

    void renderUI(Magnum::GL::Texture2D& texture);

    void begin();
    void end();

    ImGuiIntegration::Context& GetIntegration() { return _integration; }

private:
    ImGuiIntegration::Context _integration{NoCreate};
    Scene* _scene;
};
} // namespace Ham