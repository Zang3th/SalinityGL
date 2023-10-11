#pragma once

#include "glm.hpp"
#include "ErrorManager.hpp"
#include "FileManager.hpp"
#include "Types.hpp"

#include <unordered_map>

namespace Engine
{
    class Shader
    {
        private:
            uint32                                  _shaderID;
            std::unordered_map<std::string, int32>  _uniformLocationCache;

            static uint32 Compile(uint32 shaderType, const std::string& source);
            static uint32 Build(uint32 vsID, uint32 fsID);
            static uint32 Create(const std::string& vsFilepath, const std::string& fsFilepath);
            int32 GetUniformLocation(const std::string& name);

        public:
            Shader(const std::string& vsFilepath, const std::string& fsFilepath);
            ~Shader();

            void Bind() const;
            void Unbind() const;
            void SetUniform1i(const std::string& name, int32 value);
            void SetUniform1f(const std::string& name, float value);
            void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
            void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
            void SetUniformVec2f(const std::string& name, const glm::vec2& vec);
            void SetUniformVec3f(const std::string& name, const glm::vec3& vec);
            void SetUniformVec4f(const std::string& name, const glm::vec4& vec);

            [[nodiscard]] uint32 GetShaderID() const;
    };
}