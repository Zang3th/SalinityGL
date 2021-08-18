#include "Model.hpp"

namespace Core
{
    // ----- Private -----

    Ref<VertexArray> Model::CreateVaoFromMesh(Mesh* mesh)
    {
        //Create and bind vao
        Ref<VertexArray> vao = MakeRef<VertexArray>();
        vao->Bind();

        //Set vertice count accordingly
        _verticeCount = mesh->indices.size() * 3;

        //Create vbo's, send it data and configure vao
        VertexBuffer vbo1(&mesh->vertices[0], mesh->vertices.size() * sizeof(glm::vec3));
        vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

        VertexBuffer vbo2(&mesh->texCoords[0], mesh->texCoords.size() * sizeof(glm::vec2));
        vao->DefineAttributes(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

        VertexBuffer vbo3(&mesh->normals[0], mesh->normals.size() * sizeof(glm::vec3));
        vao->DefineAttributes(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

        //Create ibo
        IndexBuffer ibo(&mesh->indices[0], mesh->indices.size() * sizeof(glm::uvec3));

        //Unbind vao
        vao->Unbind();

        return vao;
    }

    void Model::SetModelMatrix()
    {
        glm::mat4 model(1.0f);

        //Translate
        model = glm::translate(model, _position);

        //Rotate
        model = glm::rotate(model, glm::radians(_rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(_rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(_rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));

        //Scale
        model = glm::scale(model, glm::vec3(_size));

        _model = model;
    }

    // ----- Public -----

    Model::Model(const Texture* texture, Shader* shader, Mesh* mesh)
        :   _model(glm::mat4(1.0f)),
            _position(0.0f),
            _texture(texture),
            _shader(shader),
            _verticeCount(0),
            _rotationX(0.0f), _rotationY(0.0f), _rotationZ(0.0f),
            _size(1.0f)
    {
        _vao = CreateVaoFromMesh(mesh);
    }

    unsigned int Model::Draw(const glm::mat4& projMatrix, const glm::mat4& viewMatrix, const glm::vec3& camPos) const
    {
        _shader->Bind();

        //Set uniforms
        _shader->SetUniformMat4f("view", viewMatrix);
        _shader->SetUniformMat4f("model", _model);
        _shader->SetUniformMat4f("projection", projMatrix);
        _shader->SetUniformVec3f("viewPos", camPos);

        _texture->Bind();
        _vao->Bind();

        //Render model
        GLCall(glDrawElements(GL_TRIANGLES, _verticeCount, GL_UNSIGNED_INT, nullptr));

        _vao->Unbind();
        _texture->Unbind();
        _shader->Unbind();

        //Return rendered vertices
        return _verticeCount;
    }

    void Model::IncreasePosition(const glm::vec3& position)
    {
        _position += position;
        SetModelMatrix();
    }

    void Model::IncreaseRotation(const float& rotX, const float& rotY, const float& rotZ)
    {
        _rotationX += rotX;
        _rotationY += rotY;
        _rotationZ += rotZ;
        SetModelMatrix();
    }

    void Model::IncreaseSize(const float& size)
    {
        _size *= size;
        SetModelMatrix();
    }
}