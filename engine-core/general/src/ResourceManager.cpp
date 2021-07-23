#include "ResourceManager.hpp"

namespace Core
{
    void ResourceManager::LoadTexture(const std::string& name, const std::string& filepath)
    {
        _textureStorage[name] = std::make_shared<Texture>(filepath);
    }

    Ref<Texture> ResourceManager::GetTexture(const std::string& name)
    {
        return _textureStorage[name];
    }

    void ResourceManager::LoadShader(const std::string& name, const std::string& vsFilepath, const std::string& fsFilepath)
    {
        _shaderStorage[name] = std::make_shared<Shader>(vsFilepath, fsFilepath);
    }

    Ref<Shader> ResourceManager::GetShader(const std::string& name)
    {
        return _shaderStorage[name];
    }
}