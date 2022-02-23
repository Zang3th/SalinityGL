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
            Texture            *_texture1, *_texture2, *_texture3, *_texture4, *_texture5;
            uint32              _verticeCount;
            int32               _gotNormalMap;
            float               _rotationX, _rotationY, _rotationZ;
            float               _size;

            static Ref<VertexArray> CreateVaoFromMesh(Mesh* mesh);
            void                    SetModelMatrix();

        public:
            explicit                    Model(Mesh* mesh);
            [[nodiscard]] VertexArray*  GetVAO()          const;
            [[nodiscard]] glm::mat4     GetModelMatrix()  const;
            [[nodiscard]] uint32        GetVerticeCount() const;
            [[nodiscard]] int32         GotNormalMap()    const;

            void ChangePosition(const glm::vec3& position);
            void ChangeRotation(float rotX, float rotY, float rotZ);
            void ChangeSize(float size);

            void SetTexture1(Texture* texture);
            void SetTexture2(Texture* texture);
            void SetTexture3(Texture* texture);
            void SetTexture4(Texture* texture);
            void SetTexture5(Texture* texture);

            [[nodiscard]] Texture* GetTexture1() const;
            [[nodiscard]] Texture* GetTexture2() const;
            [[nodiscard]] Texture* GetTexture3() const;
            [[nodiscard]] Texture* GetTexture4() const;
            [[nodiscard]] Texture* GetTexture5() const;
    };
}