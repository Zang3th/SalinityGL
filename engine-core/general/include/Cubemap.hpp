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
            Scope<CubemapTexture> _cubemapTexture;
            Ref<VertexArray> _vao;
            Shader* _shader;
            unsigned int _verticeCount;

            Ref<VertexArray> CreateVao();

        public:
            Cubemap(std::vector<const char*>& faces, Shader* shader);
            unsigned int Draw(const glm::mat4& projMatrix, const glm::mat4& viewMatrix) const;
    };
}