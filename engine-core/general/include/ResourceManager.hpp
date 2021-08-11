#pragma once

#include <map>
#include "Texture.hpp"
#include "Shader.hpp"
#include "CoreDefines.hpp"

namespace Core
{
    class ResourceManager
    {
        public:
            inline static std::map<std::string, Texture*> _textureStorage = std::map<std::string, Texture*>();
            inline static std::map<std::string, Shader*> _shaderStorage = std::map<std::string, Shader*>();

        public:
            static void LoadTexture(const std::string& name, const std::string& filepath);
            static Texture* GetTexture(const std::string& name);
            static void LoadShader(const std::string& name, const std::string& vsFilepath, const std::string& fsFilepath);
            static Shader* GetShader(const std::string& name);
            static void CleanUp();
    };
}