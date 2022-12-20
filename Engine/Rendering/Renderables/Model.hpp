#pragma once

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Types.hpp"
#include "ErrorManager.hpp"
#include "Mesh.hpp"

#include <vector>

namespace Engine
{
    class Model
    {
        private:
            Scope<VertexArray>    _vao;
            Scope<VertexBuffer>   _vboVert, _vboTex, _vboNorm, _vboTang;
            Scope<IndexBuffer>    _ibo;
            glm::mat4             _model;
            glm::vec3             _position;
            std::vector<Texture*> _textures;
            uint32                _verticeCount;
            int32                 _gotNormalMap;
            float                 _rotationX, _rotationY, _rotationZ;
            float                 _size;

            void                  InitGpuStorage(Mesh* mesh);
            void                  SetModelMatrix();

        public:
            explicit                    Model(Mesh* mesh);
            [[nodiscard]] glm::mat4     GetModelMatrix()  const;
            [[nodiscard]] uint32        GetVerticeCount() const;
            [[nodiscard]] int32         GotNormalMap()    const;

            void BindBuffers() const;
            void UnbindBuffers() const;
            void ChangePosition(const glm::vec3& position);
            void ChangeRotation(float rotX, float rotY, float rotZ);
            void ChangeSize(float size);
            void AddTexture(Texture* texture);
            [[nodiscard]] const std::vector<Texture*>* GetTextures() const;
    };
}