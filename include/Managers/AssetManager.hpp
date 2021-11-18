#pragma once

#include "Managers/AssetLoader.hpp"

namespace Ham
{


class AssetManager
{
// public:
//     static Component::Shader& LoadShader(const char* unique_name, std::filesystem::path path, Diligent::SHADER_TYPE type)
//     {
//         // ::Ham::Shader s;
//         _shaders.insert({std::string(unique_name), {}});
//         auto& sh = _shaders.at(std::string(unique_name));

//         sh.Info.SourceLanguage             = Diligent::SHADER_SOURCE_LANGUAGE_HLSL;
//         sh.Info.UseCombinedTextureSamplers = true;
//         sh.Info.Desc.ShaderType            = type;
//         sh.Info.Desc.Name                  = unique_name;
//         sh.Source                          = AssetLoader::ReadShaderFile(path);
//         sh.Info.Source                     = sh.Source.c_str();
//         sh.Info.FilePath                   = NULL;
//         sh.Info.ByteCode                   = NULL;

//         return sh;
//     }

//     static std::string& LoadTexture(std::string name, std::filesystem::path path)
//     {
//         std::string result;
//         _textures.insert(std::pair<std::string, std::string>(name, AssetLoader::ReadShaderFile(path)));
//         return _textures.at(name);
//     }

//     static Diligent::ShaderCreateInfo& GetShader(std::string name) { return _shaders.at(name); }
//     static auto&                       GetTexture(std::string name) { return _textures.at(name); }

//     static void UnloadShader(std::string name) { _shaders.erase(name); }
//     static void UnloadTexture(std::string name) { _textures.erase(name); }

// private:
//     static std::unordered_map<std::string, ::Ham::Component::Shader> _shaders;
//     static std::unordered_map<std::string, std::string>              _textures;
};
} // namespace Ham::Asset