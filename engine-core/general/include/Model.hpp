#pragma once

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "CoreDefines.hpp"
#include "ErrorManager.hpp"
#include "Mesh.hpp"

namespace Core
{
    class Model
    {
        private:
            Ref<VertexArray> _vao;
            Texture* _texture;
            Shader* _shader;
            glm::vec3 _color;
            glm::mat4 _model;
            unsigned int _verticeCount;

            Ref<VertexArray> CreateVaoFromMesh(Mesh* mesh);

        public:
            Model(Texture* texture, Shader* shader, glm::vec3 color, Mesh* mesh);
            unsigned int Draw(const glm::mat4& projMatrix, const glm::mat4& viewMatrix, const glm::vec3& camPos);
    };
}