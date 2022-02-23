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
            _texture1(mesh->texture1),
            _texture2(mesh->texture2),
            _texture3(mesh->texture3),
            _texture4(mesh->texture4),
            _texture5(mesh->texture5),
            _verticeCount(mesh->indices.size()),
            _gotNormalMap(mesh->gotNormalMap),
            _rotationX(0.0f), _rotationY(0.0f), _rotationZ(0.0f),
            _size(1.0f)
    {

    }

    VertexArray* Model::GetVAO() const
    {
        return _vao.get();
    }

    glm::mat4 Model::GetModelMatrix() const
    {
        return _model;
    }

    uint32 Model::GetVerticeCount() const
    {
        return _verticeCount;
    }

    int32 Model::GotNormalMap() const
    {
        return _gotNormalMap;
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

    void Model::SetTexture1(Texture* texture)
    {
        _texture1 = texture;
    }

    void Model::SetTexture2(Texture* texture)
    {
        _texture2 = texture;
    }

    void Model::SetTexture3(Texture* texture)
    {
        _texture3 = texture;
    }

    void Model::SetTexture4(Texture* texture)
    {
        _texture4 = texture;
    }

    void Model::SetTexture5(Texture* texture)
    {
        _texture5 = texture;
    }

    Texture* Model::GetTexture1() const
    {
        return _texture1;
    }

    Texture* Model::GetTexture2() const
    {
        return _texture2;
    }

    Texture* Model::GetTexture3() const
    {
        return _texture3;
    }

    Texture* Model::GetTexture4() const
    {
        return _texture4;
    }

    Texture* Model::GetTexture5() const
    {
        return _texture5;
    }
}