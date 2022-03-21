#pragma once

#include "Texture.hpp"
#include "Shader.hpp"
#include "CoreDefines.hpp"

#include <map>

namespace Engine
{
    class ResourceManager
    {
        public:
            inline static std::map<std::string, Texture*> _textureStorage = std::map<std::string, Texture*>();
            inline static std::map<std::string, Shader*>  _shaderStorage  = std::map<std::string, Shader*>();

        public:
            ResourceManager() = delete;
            static Texture* LoadTexture(const std::string& name, uint32 width, uint32 height, GLint internalFormat, GLenum format, GLenum type);
            static Texture* LoadTextureFromFile(const std::string& name, const std::string& filepath);
            static Texture* LoadTextureAtlasFromFile(const std::string& name, const std::string& filepath, uint32 numberOfRows);
            static Texture* GetTexture(const std::string& name);
            static std::string OutputTextureStorage();
            static Shader* LoadShader(const std::string& name, const std::string& vsFilepath, const std::string& fsFilepath);
            static Shader* GetShader(const std::string& name);
            static std::string OutputShaderStorage();
            static void CleanUp();
    };
}