#pragma once

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Types.hpp"
#include "GlobalParams.hpp"

namespace Engine
{
    class Sprite
    {
        private:
            Scope<VertexArray>  _vao;
            Scope<VertexBuffer> _vboVert;
            glm::mat4           _orthoProj;
            glm::mat4           _model;
            glm::vec3           _color;
            Texture*            _texture;
            Shader*             _shader;
            glm::vec2           _position;
            glm::vec2           _size;
            float               _rotation;
            uint32              _verticeCount;

            void                InitGpuStorage();
            void                SetModelMatrix();

        public:
            Sprite(Texture* texture, Shader* shader, const glm::vec3& color, const glm::vec2& size);

            [[nodiscard]] uint32 Draw() const;

            void ChangePosition(const glm::vec2& position);
            void ChangeRotation(float rotation);
            void ChangeSize(const glm::vec2& size);
            void SetTexture(Texture* texture);
    };
}