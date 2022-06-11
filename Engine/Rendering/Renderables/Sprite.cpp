#include "Sprite.hpp"

namespace Engine
{
    // ----- Private -----   

    Ref<VertexArray> Sprite::CreateSpriteVao()
    {
        //Create and bind vao
        Ref<VertexArray> vao = MakeRef<VertexArray>();
        vao->Bind();

        //Create data
        static const float vertices[] =
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
        VertexBuffer vbo(vertices, sizeof(vertices), GL_STATIC_DRAW);
        vao->DefineAttributes(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

        //Unbind vao
        vao->Unbind();

        return vao;
    }
    
    void Sprite::SetModelMatrix()
    {
        //Create model matrix and apply transformations
        glm::mat4 model(1.0f);

        //Translate
        model = glm::translate(model, glm::vec3(_position, 0.0f));

        //Rotate
        model = glm::translate(model, glm::vec3(0.5f * _size.x, 0.5f * _size.y, 0.0f));
        model = glm::rotate(model, glm::radians(_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * _size.x, -0.5f * _size.y, 0.0f));

        //Scale
        model = glm::scale(model, glm::vec3(_size, 1.0f));

        _model = model;
    }

    // ----- Public -----

    Sprite::Sprite(Texture* texture, Shader* shader, glm::vec3 color)
        :   _vao(CreateSpriteVao()),
            _model(glm::mat4(1.0f)),
            _color(color),
            _texture(texture),
            _shader(shader),
            _position(glm::vec2(0.0f)),
            _size(glm::vec2(1.0f)),
            _rotation(0.0f),
            _verticeCount(6)
    {}

    uint32 Sprite::Draw(const glm::mat4& projMatrix) const
    {
        _shader->Bind();
        
        //Set uniforms
        _shader->SetUniformVec3f("color", _color);
        _shader->SetUniformMat4f("model", _model);
        _shader->SetUniformMat4f("projection", projMatrix);

        _texture->Bind();
        _vao->Bind();

        //Render quad
        GLCall(glDrawArrays(GL_TRIANGLES, 0, _verticeCount));

        _vao->Unbind();
        _texture->Unbind();
        _shader->Unbind();

        //Return rendered vertices
        return _verticeCount;
    }

    void Sprite::ChangePosition(const glm::vec2& position)
    {
        _position = position;
        SetModelMatrix();
    }

    void Sprite::ChangeRotation(const float rotation)
    {
        _rotation = rotation;
        SetModelMatrix();
    }

    void Sprite::ChangeSize(const glm::vec2& size)
    {
        _size = size;
        SetModelMatrix();
    }

    void Sprite::SetTexture(Texture* texture)
    {
        _texture = texture;
    }
}