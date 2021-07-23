#pragma once

#include <glm/glm.hpp>
#include "ErrorManager.hpp"
#include <string>
#include "FileManager.hpp"
#include "CoreDefines.hpp"

namespace Core
{
    class Shader
    {
        private:
            unsigned int _shaderID;
            std::unordered_map<std::string, int> _uniformLocationCache;

            int Compile(unsigned int shaderType, const std::string& source);
            int Build(unsigned int vsID, unsigned int fsID);
            int Create(const std::string& vsFilepath, const std::string& fsFilepath);
            int GetUniformLocation(const std::string& name);

        public:
            Shader(const std::string& vsFilepath, const std::string& fsFilepath);
            ~Shader();
            void Bind();
            void Unbind();    
            void SetUniform1i(const std::string& name, int value);
            void SetUniform1f(const std::string& name, float value);
            void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
            void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
            void SetUniformVec3f(const std::string& name, const glm::vec3& vec);
    };
}