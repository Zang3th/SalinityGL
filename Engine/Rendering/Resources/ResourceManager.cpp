#include "ResourceManager.hpp"

namespace Engine
{
    // ----- Public -----

    GLTexture* ResourceManager::LoadGLTexture(const std::string& name, const std::string& filepath)
    {
        auto* glTexture = new GLTexture(filepath, false);
        _glTextureStorage[name] = glTexture ;
        return glTexture ;
    }

    GLTexture* ResourceManager::LoadGLTextureAtlas(const std::string& name, const std::string& filepath, uint32 numberOfRows)
    {
        auto* glTexture = new GLTexture(filepath, false, numberOfRows);
        _glTextureStorage[name] = glTexture ;
        return glTexture ;
    }

    GLTexture* ResourceManager::LoadGLTextureWithBackup(const std::string& name, const std::string& filepath)
    {
        auto* glTexture = new GLTexture(filepath, true);
        _glTextureStorage[name] = glTexture ;
        return glTexture ;
    }

    GLTexture* ResourceManager::CreateGLTexture(const std::string& name, const uint32 width, const uint32 height, GLint internalFormat, GLenum format, GLenum type)
    {
        auto* glTexture = new GLTexture(width, height, internalFormat, format, type);
        _glTextureStorage[name] = glTexture ;
        return glTexture;
    }

    GLTexture* ResourceManager::GetGLTexture(const std::string& name)
    {
        return _glTextureStorage[name];
    }

    std::string ResourceManager::OutputGLTextureStorage()
    {
        std::string output;

        for(auto const& glTex : _glTextureStorage)
        {
            output += (glTex .first + "\n");
        }

        return output;
    }

    TextureBuffer* ResourceManager::LoadTextureBuffer(const std::string& name, const std::string& filepath)
    {
        auto* texBuffer = new TextureBuffer(filepath, false);
        _texBufferStorage[name] = texBuffer;
        return texBuffer;
    }

    TextureBuffer* ResourceManager::LoadTextureBufferWithBackup(const std::string& name, const std::string& filepath)
    {
        auto* texBuffer = new TextureBuffer(filepath, true);
        _texBufferStorage[name] = texBuffer;
        return texBuffer;
    }

    TextureBuffer* ResourceManager::GetTextureBuffer(const std::string& name)
    {
        return _texBufferStorage[name];
    }

    Shader* ResourceManager::LoadShader(const std::string& name, const std::string& vsFilepath, const std::string& fsFilepath)
    {
        auto* shader = new Shader(vsFilepath, fsFilepath);
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

        for(auto const& shader : _shaderStorage)
        {
            output += (shader.first + "\n");
        }

        return output;
    }

    Heightmap* ResourceManager::LoadHeightmap(const std::string& name, const std::string& filepath)
    {
        auto* heightmap = new Heightmap(filepath);
        _heightmapStorage[name] = heightmap;
        return heightmap;
    }

    Heightmap* ResourceManager::GetHeightmap(const std::string& name)
    {
        return _heightmapStorage[name];
    }

    void ResourceManager::CleanUp()
    {
        for(auto const& glTex : _glTextureStorage)
        {
            delete glTex.second;
        }

        for(auto const& texBuffer : _texBufferStorage)
        {
            delete texBuffer .second;
        }

        for(auto const& shader : _shaderStorage)
        {
            delete shader.second;
        }
    }
}
