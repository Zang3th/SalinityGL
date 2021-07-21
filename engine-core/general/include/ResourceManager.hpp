#pragma once

#include <map>
#include "Texture.hpp"
#include "Shader.hpp"

class ResourceManager
{
    private:
        static std::map<std::string, Texture*> _textureStorage;
        static std::map<std::string, Shader*> _shaderStorage;

        ResourceManager(){}

    public:
        static void LoadTexture(const std::string& name, const std::string& filepath);
        static Texture* GetTexture(const std::string& name);
        static void LoadShader(const std::string& name, const std::string& vsFilepath, const std::string& fsFilepath);
        static Shader* GetShader(const std::string& name);
        static void CleanUp();
};