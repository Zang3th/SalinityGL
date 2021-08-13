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
            glm::mat4 _model;
            unsigned int _verticeCount;
            glm::vec3 _position;
            float _rotationX, _rotationY, _rotationZ;
            float _size;

            Ref<VertexArray> CreateVaoFromMesh(Mesh* mesh);
            void SetModelMatrix();

        public:
            Model(Texture* texture, Shader* shader, Mesh* mesh);
            unsigned int Draw(const glm::mat4& projMatrix, const glm::mat4& viewMatrix, const glm::vec3& camPos) const;
            void IncreasePosition(const glm::vec3& position);
            void IncreaseRotation(float rotX, float rotY,float rotZ);
            void IncreaseSize(float size);
    };
}