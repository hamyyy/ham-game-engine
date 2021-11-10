#pragma once

#include "Core/Core.hpp"
#include "Scene/Layer.hpp"

namespace Ham
{
class LayerManager
{
public:
    static void Initialize()
    {
    }

    static void PushLayer(Layer* layer);
    static void PopLayer(Layer* layer);

    static void PushOverlay(Layer* layer);
    static void PopOverlay(Layer* layer);

    static void RemoveLayer(uint32_t index);
    static void InsertLayer(Layer* layer, uint32_t index);


    static void RemoveAllLayers();

    static void UpdateLayers();

    static Layer* GetLayer(int index);
    static Layer* GetLayer(std::string name);

    static size_t GetLayerCount() { return _layers.size(); }

private:
    static std::vector<Layer*> _layers;
    static uint32_t            _layerInsertIndex;
};

} // namespace Ham