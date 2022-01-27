#include "ResourceManager.hpp"

namespace Core
{
    // ----- Public -----

    void ResourceManager::LoadTexture(const std::string& name, const uint32 width, const uint32 height, GLenum format)
    {
        Texture* texture = new Texture(width, height, format);
        _textureStorage[name] = texture;
    }

    void ResourceManager::LoadTextureFromFile(const std::string& name, const std::string& filepath)
    {
        Texture* texture = new Texture(filepath);
        _textureStorage[name] = texture;
    }

    Texture* ResourceManager::GetTexture(const std::string& name)
    {
        return _textureStorage[name];
    }

    std::string ResourceManager::OutputTextureStorage()
    {
        std::string output;

        for (auto const& tex : _textureStorage)
            output += std::to_string(tex.second->GetTextureID()) + ": " + tex.first + "\n";

        return output;
    }

    void ResourceManager::LoadShader(const std::string& name, const std::string& vsFilepath, const std::string& fsFilepath)
    {
        _shaderStorage[name] = new Shader(vsFilepath, fsFilepath);
    }

    Shader* ResourceManager::GetShader(const std::string& name)
    {
        return _shaderStorage[name];
    }

    std::string ResourceManager::OutputShaderStorage()
    {
        std::string output;

        for (auto const& shader : _shaderStorage)
            output += std::to_string(shader.second->GetShaderID()) + ": " + shader.first + "\n";

        return output;
    }

    void ResourceManager::CleanUp()
    {
        for (auto const& tex : _textureStorage)
            delete tex.second;

        for (auto const& shader : _shaderStorage)
            delete shader.second;
    }
}