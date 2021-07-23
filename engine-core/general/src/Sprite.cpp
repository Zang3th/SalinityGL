#include "Sprite.hpp"

namespace Core
{
    // ----- Private -----   

    Ref<VertexArray> Sprite::CreateSpriteVao()
    {
        //Create and bind vao
        Ref<VertexArray> vao = std::make_shared<VertexArray>();
        vao->Bind();

        //Create data
        float vertices[] =
        {
            //Position  //Texture
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        };

        //Create vbo, send it data and configure vao
        VertexBuffer vbo(vertices, sizeof(vertices));
        vao->DefineAttributes(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

        //Unbind vao
        vao->Unbind();

        return vao;
    }
    
    glm::mat4 Sprite::CreateModelMatrix(glm::vec2 position, glm::vec2 size, float rotation)
    {
        //Create model matrix and apply transformations
        glm::mat4 model = glm::mat4(1.0f);

        //Translate
        model = glm::translate(model, glm::vec3(position, 0.0f));

        //Rotate
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

        //Scale
        model = glm::scale(model, glm::vec3(size, 1.0f));

        return model;
    }

    // ----- Public -----

    Sprite::Sprite(Ref<Texture> texture, Ref<Shader> shader, glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color)
        : _texture(texture), _shader(shader), _position(position), _size(size), _rotation(rotation), _color(color)
    {
        _vao = CreateSpriteVao();
        _model = CreateModelMatrix(_position, _size, _rotation);
    }

    void Sprite::TranslateSprite(glm::vec2 position)
    {
        _model = CreateModelMatrix(position, _size, _rotation);
    }
}