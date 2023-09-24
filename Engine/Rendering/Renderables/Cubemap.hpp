#pragma once

#include "CubemapTexture.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Types.hpp"
#include "ErrorManager.hpp"

namespace Engine
{
    class Cubemap
    {
        private:
            uint32                  _verticeCount;
            Shader*                 _shader;
            Scope<VertexArray>      _vao;
            Scope<VertexBuffer>     _vboVert;
            Scope<CubemapTexture>   _cubemapTexture;

            void                    InitGpuStorage();

        public:
            Cubemap(const std::array<const char*, 6>& faces, Shader* shader);
            [[nodiscard]] uint32 Draw(const glm::mat4& projMatrix, const glm::mat4& viewMatrix) const;
    };
}