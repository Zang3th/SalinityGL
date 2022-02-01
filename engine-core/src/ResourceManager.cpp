#include "ResourceManager.hpp"

namespace Core
{
    // ----- Public -----

    Texture* ResourceManager::LoadTexture(const std::string& name, const uint32 width, const uint32 height, GLenum format)
    {
        Texture* texture = new Texture(width, height, format);
        _textureStorage[name] = texture;
        return texture;
    }

    Texture* ResourceManager::LoadTextureFromFile(const std::string& name, const std::string& filepath)
    {
        Texture* texture = new Texture(filepath);
        _textureStorage[name] = texture;
        return texture;
    }

    Texture* ResourceManager::GetTexture(const std::string& name)
    {
        return _textureStorage[name];
    }

    std::string ResourceManager::OutputTextureStorage()
    {
        std::string output;

        for (auto const& tex : _textureStorage)
            output += (tex.first + "\n");

        return output;
    }

    Shader* ResourceManager::LoadShader(const std::string& name, const std::string& vsFilepath, const std::string& fsFilepath)
    {
         Shader* shader = new Shader(vsFilepath, fsFilepath);
        _shaderStorage[name] = shader;
        return shader;
    }

    Shader* ResourceManager::GetShader(const std::string& name)
    {
        return _shaderStorage[name];
    }

    std::string ResourceManager::OutputShaderStorage()
    {
        std::string output;

        for (auto const& shader : _shaderStorage)
            output += (shader.first + "\n");

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