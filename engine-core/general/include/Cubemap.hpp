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
            unsigned int            _verticeCount;

            Ref<VertexArray> CreateVao();

        public:
            Cubemap(std::array<const char*, 6>& faces, Shader* shader);
            [[nodiscard]] unsigned int Draw(const glm::mat4& projMatrix, const glm::mat4& viewMatrix) const;
    };
}