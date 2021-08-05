#include "ResourceManager.hpp"

namespace Core
{
    void ResourceManager::LoadTexture(const std::string& name, const std::string& filepath)
    {
        _textureStorage[name] = MakeScope<Texture>(filepath);
    }

    Texture* ResourceManager::GetTexture(const std::string& name)
    {
        return _textureStorage[name].get();
    }

    void ResourceManager::LoadShader(const std::string& name, const std::string& vsFilepath, const std::string& fsFilepath)
    {
        _shaderStorage[name] = MakeScope<Shader>(vsFilepath, fsFilepath);
    }

    Shader* ResourceManager::GetShader(const std::string& name)
    {
        return _shaderStorage[name].get();
    }
}