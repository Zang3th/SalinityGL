#pragma once

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "CoreDefines.hpp"
#include "ErrorManager.hpp"

namespace Core
{
    class Sprite
    {
        private:
            Ref<VertexArray>    _vao;
            glm::mat4           _model;
            glm::vec3           _color;
            const Texture*      _texture;
            Shader*             _shader;
            glm::vec2           _position;
            glm::vec2           _size;
            float               _rotation;
            uint32              _verticeCount;

            Ref<VertexArray> CreateSpriteVao();
            static glm::mat4 CreateModelMatrix(glm::vec2 position, glm::vec2 size, float rotation);

        public:
            Sprite(const Texture* texture, Shader* shader, glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color);
            [[nodiscard]] uint32 Draw(const glm::mat4& projection) const;
            void Translate(const glm::vec2& position);
    };
}