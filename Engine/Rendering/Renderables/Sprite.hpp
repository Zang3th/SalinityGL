#pragma once

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "CoreDefines.hpp"
#include "ErrorManager.hpp"

namespace Engine
{
    class Sprite
    {
        private:
            Ref<VertexArray>    _vao;
            glm::mat4           _model;
            glm::vec3           _color;
            Texture*            _texture;
            Shader*             _shader;
            glm::vec2           _position;
            glm::vec2           _size;
            float               _rotation;
            uint32              _verticeCount;

            static Ref<VertexArray> CreateSpriteVao();
            void SetModelMatrix();

        public:
            Sprite(Texture* texture, Shader* shader, glm::vec3 color);
            [[nodiscard]] uint32 Draw(const glm::mat4& projection) const;
            void ChangePosition(const glm::vec2& position);
            void ChangeRotation(float rotation);
            void ChangeSize(const glm::vec2& size);
            void SetTexture(Texture* texture);
    };
}