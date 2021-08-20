#include "ResourceManager.hpp"

namespace Core
{
    // ----- Public -----

    void ResourceManager::LoadTexture(const std::string& name, const std::string& filepath)
    {
        _textureStorage[name] = new Texture(filepath);
    }

    Texture* ResourceManager::GetTexture(const std::string& name)
    {
        return _textureStorage[name];
    }

    void ResourceManager::LoadShader(const std::string& name, const std::string& vsFilepath, const std::string& fsFilepath)
    {
        _shaderStorage[name] = new Shader(vsFilepath, fsFilepath);
    }

    Shader* ResourceManager::GetShader(const std::string& name)
    {
        return _shaderStorage[name];
    }

    void ResourceManager::CleanUp()
    {
        for (auto const& tex : _textureStorage)
            delete tex.second;

        for (auto const& shader : _shaderStorage)
            delete shader.second;
    }
}