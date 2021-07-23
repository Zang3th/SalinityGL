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
            std::map<std::string, Ref<Texture>> _textureStorage;
            std::map<std::string, Ref<Shader>> _shaderStorage;

        public:
            void LoadTexture(const std::string& name, const std::string& filepath);
            Ref<Texture> GetTexture(const std::string& name);
            void LoadShader(const std::string& name, const std::string& vsFilepath, const std::string& fsFilepath);
            Ref<Shader> GetShader(const std::string& name);
    };
}