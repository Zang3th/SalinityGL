#pragma once

#include "GLTexture.hpp"
#include "TextureBuffer.hpp"
#include "Shader.hpp"
#include "Heightmap.hpp"
#include "Types.hpp"

#include <map>

namespace Engine
{
    class ResourceManager
    {
        public:
            inline static std::map<std::string, GLTexture*>     _glTextureStorage = std::map<std::string, GLTexture*>();
            inline static std::map<std::string, TextureBuffer*> _texBufferStorage = std::map<std::string, TextureBuffer*>();
            inline static std::map<std::string, Shader*>        _shaderStorage    = std::map<std::string, Shader*>();
            inline static std::map<std::string, Heightmap*>     _heightmapStorage = std::map<std::string, Heightmap*>();

            ResourceManager() = delete;

            static GLTexture* LoadGLTexture(const std::string& name, const std::string& filepath);
            static GLTexture* LoadGLTextureAtlas(const std::string& name, const std::string& filepath, uint32 numberOfRows);
            static GLTexture* LoadGLTextureWithBackup(const std::string& name, const std::string& filepath);
            static GLTexture* CreateGLTexture(const std::string& name, uint32 width, uint32 height, GLint internalFormat, GLenum format, GLenum type);
            static GLTexture* CreateGLTextureFromBuffer(const std::string& name, const TextureBuffer* texBuffer);
            static GLTexture* GetGLTexture(const std::string& name);
            static std::string OutputGLTextureStorage();

            static TextureBuffer* LoadTextureBuffer(const std::string& name, const std::string& filepath);
            static TextureBuffer* LoadTextureBufferWithBackup(const std::string& name, const std::string& filepath);
            static TextureBuffer* GetTextureBuffer(const std::string& name);

            static Shader* LoadShader(const std::string& name, const std::string& vsFilepath, const std::string& fsFilepath);
            static Shader* GetShader(const std::string& name);
            static std::string OutputShaderStorage();

            static Heightmap* LoadHeightmap(const std::string& name, const std::string& filepath);
            static Heightmap* GetHeightmap(const std::string& name);

            static void CleanUp();
    };
}
