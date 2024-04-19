#pragma once

#include "Texture.hpp"
#include "Shader.hpp"
#include "Heightmap.hpp"
#include "Types.hpp"

#include <map>

namespace Engine
{
    class ResourceManager
    {
        public:
            inline static std::map<std::string, Texture*>   _textureStorage   = std::map<std::string, Texture*>();
            inline static std::map<std::string, Shader*>    _shaderStorage    = std::map<std::string, Shader*>();
            inline static std::map<std::string, Heightmap*> _heightmapStorage = std::map<std::string, Heightmap*>();

            ResourceManager() = delete;

            static Texture* CreateTexture(const std::string& name, uint32 width, uint32 height, GLint internalFormat, GLenum format, GLenum type);
            static Texture* LoadTexture(const std::string& name, const std::string& filepath);
            static Texture* LoadTextureToBuffer(const std::string& name, const std::string& filepath);
            static Texture* LoadTextureAtlas(const std::string& name, const std::string& filepath, uint32 numberOfRows);
            static Texture* GetTexture(const std::string& name);
            static std::string OutputTextureStorage();

            static Shader* LoadShader(const std::string& name, const std::string& vsFilepath, const std::string& fsFilepath);
            static Shader* GetShader(const std::string& name);
            static std::string OutputShaderStorage();

            static Heightmap* LoadHeightmap(const std::string& name, const std::string& filepath);
            static Heightmap* GetHeightmap(const std::string& name);

            static void CleanUp();
    };
}