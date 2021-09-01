#include "Model.hpp"

namespace Core
{
    // ----- Private -----

    Ref<VertexArray> Model::CreateVaoFromMesh(Mesh* mesh)
    {
        //Create and bind vao
        Ref<VertexArray> vao = MakeRef<VertexArray>();
        vao->Bind();

        //Create vbo's, send it data and configure vao
        VertexBuffer vbo1(&mesh->vertices[0], mesh->vertices.size() * sizeof(glm::vec3));
        vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

        VertexBuffer vbo2(&mesh->texCoords[0], mesh->texCoords.size() * sizeof(glm::vec2));
        vao->DefineAttributes(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

        VertexBuffer vbo3(&mesh->normals[0], mesh->normals.size() * sizeof(glm::vec3));
        vao->DefineAttributes(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

        //Create ibo
        IndexBuffer ibo(&mesh->indices[0], mesh->indices.size() * sizeof(uint32));

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

    Model::Model(Mesh* mesh)
        :   _vao(CreateVaoFromMesh(mesh)),
            _model(glm::mat4(1.0f)),
            _position(0.0f),
            _diffuseTexture(mesh->diffuseTexture),
            _normalMap(mesh->normalMap),
            _verticeCount(mesh->indices.size()),
            _gotNormalMap(mesh->gotNormalMap),
            _rotationX(0.0f), _rotationY(0.0f), _rotationZ(0.0f),
            _size(1.0f)
    {

    }

    uint32 Model::Draw(Shader* shader, const glm::mat4& projMatrix, const glm::mat4& viewMatrix, const glm::vec3& camPos) const
    {
        shader->Bind();

        //Set uniforms
        shader->SetUniformMat4f("view", viewMatrix);
        shader->SetUniformMat4f("model", _model);
        shader->SetUniformMat4f("projection", projMatrix);
        shader->SetUniformVec3f("viewPos", camPos);
        shader->SetUniform1i("diffuseTexture", 0);
        shader->SetUniform1i("normalMap", 1);
        shader->SetUniform1i("gotNormalMap", _gotNormalMap);

        _diffuseTexture->BindToSlot(0);

        if(_gotNormalMap == 1)
            _normalMap->BindToSlot(1);

        _vao->Bind();

        //Render model
        GLCall(glDrawElements(GL_TRIANGLES, _verticeCount, GL_UNSIGNED_INT, nullptr));

        _vao->Unbind();
        _diffuseTexture->Unbind();
        shader->Unbind();

        //Return rendered vertices
        return _verticeCount;
    }

    void Model::ChangePosition(const glm::vec3& position)
    {
        _position += position;
        SetModelMatrix();
    }

    void Model::ChangeRotation(const float rotX, const float rotY, const float rotZ)
    {
        _rotationX += rotX;
        _rotationY += rotY;
        _rotationZ += rotZ;
        SetModelMatrix();
    }

    void Model::ChangeSize(const float size)
    {
        _size *= size;
        SetModelMatrix();
    }
}