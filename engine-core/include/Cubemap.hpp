#pragma once

#include "CubemapTexture.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "CoreDefines.hpp"
#include "ErrorManager.hpp"

namespace Core
{
    class Cubemap
    {
        private:
            Ref<VertexArray>        _vao;
            Scope<CubemapTexture>   _cubemapTexture;
            Shader*                 _shader;
            uint32                  _verticeCount;

            Ref<VertexArray> CreateVao();

        public:
            Cubemap(std::array<const std::string, 6>& faces, Shader* shader);
            [[nodiscard]] uint32 Draw(const glm::mat4& projMatrix, const glm::mat4& viewMatrix) const;
    };
}