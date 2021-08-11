#pragma once

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "CoreDefines.hpp"
#include "ErrorManager.hpp"

namespace Core
{
    class Sprite
    {
        private:
            Ref<VertexArray> _vao;
            Texture* _texture;
            Shader* _shader;
            glm::vec2 _position;
            glm::vec2 _size;
            float _rotation;
            glm::vec3 _color;
            glm::mat4 _model;
            unsigned int _verticeCount;

            Ref<VertexArray> CreateSpriteVao();
            glm::mat4 CreateModelMatrix(glm::vec2 position, glm::vec2 size, float rotation);

        public:
            Sprite(Texture* texture, Shader* shader, glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color);
            unsigned int Draw(const glm::mat4& projection) const;
            void Translate(glm::vec2 position);
    };
}