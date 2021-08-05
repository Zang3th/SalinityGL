#pragma once

#include <map>
#include "Texture.hpp"
#include "Shader.hpp"
#include "CoreDefines.hpp"

namespace Core
{
    class ResourceManager
    {
        private:
            std::map<std::string, Scope<Texture>> _textureStorage;
            std::map<std::string, Scope<Shader>> _shaderStorage;

        public:
            void LoadTexture(const std::string& name, const std::string& filepath);
            Texture* GetTexture(const std::string& name);
            void LoadShader(const std::string& name, const std::string& vsFilepath, const std::string& fsFilepath);
            Shader* GetShader(const std::string& name);
    };
}