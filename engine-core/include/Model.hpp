#pragma once

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "CoreDefines.hpp"
#include "ErrorManager.hpp"
#include "Mesh.hpp"

namespace Core
{
    class Model
    {
        private:
            Ref<VertexArray>    _vao;
            glm::mat4           _model;
            glm::vec3           _position;
            const Texture      *_texture1, *_texture2, *_texture3;
            uint32              _verticeCount;
            int32               _gotNormalMap;
            float               _rotationX, _rotationY, _rotationZ;
            float               _size;

            static Ref<VertexArray> CreateVaoFromMesh(Mesh* mesh);
            void SetModelMatrix();

        public:
            explicit Model(Mesh* mesh);
            [[nodiscard]] uint32 DrawModel(Shader* shader, const glm::mat4& projMatrix, const glm::mat4& viewMatrix, const glm::vec3& camPos, const glm::mat4& lightProjection) const;
            [[nodiscard]] uint32 DrawWaterModel(Shader* shader, const glm::mat4& projMatrix, const glm::mat4& viewMatrix, const glm::vec3& camPos) const;
            void ChangePosition(const glm::vec3& position);
            void ChangeRotation(float rotX, float rotY, float rotZ);
            void ChangeSize(float size);
    };
}