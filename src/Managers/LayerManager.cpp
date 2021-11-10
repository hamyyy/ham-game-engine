#include "Managers/LayerManager.hpp"

namespace Ham
{

std::vector<Layer*> LayerManager::_layers = {};
uint32_t            LayerManager::_layerInsertIndex = 0;

void LayerManager::PushLayer(Layer* layer)
{
    _layers.insert(_layers.begin() + _layerInsertIndex, layer);
    layer->start();
}

void LayerManager::PopLayer(Layer* layer)
{
    auto it = std::find(_layers.begin(), _layers.end(), layer);
    if (it != _layers.end())
    {
        _layers.erase(it);
        _layerInsertIndex--;
    }
}

void LayerManager::PushOverlay(Layer* layer)
{
    _layers.push_back(layer);
    layer->start();
}

void LayerManager::PopOverlay(Layer* layer)
{
    auto it = std::find(_layers.begin(), _layers.end(), layer);
    if (it != _layers.end())
    {
        layer->exit();
        _layers.erase(it);
    }
}

void LayerManager::RemoveLayer(uint32_t index)
{
    _layers.at(index)->exit();
    _layers.erase(_layers.begin() + index);
}

void LayerManager::InsertLayer(Layer* layer, uint32_t index)
{
    _layers.insert(_layers.begin() + index, layer);
    layer->start();
}

void LayerManager::RemoveAllLayers()
{
    for (auto& layer : _layers)
    {
        layer->exit();
        auto it = std::find(_layers.begin(), _layers.end(), layer);
        if (it != _layers.end())
        {
            _layers.erase(it);
        }
    }
}

void LayerManager::UpdateLayers()
{
    for (auto& layer : _layers)
    {
        layer->update();
    }
}

Layer* LayerManager::GetLayer(int index)
{
    return _layers.at(index);
}

Layer* LayerManager::GetLayer(std::string name)
{
    for (auto& layer : _layers)
    {
        if (layer->GetName() == name)
        {
            return layer;
        }
    }
    return nullptr;
}

} // namespace Ham