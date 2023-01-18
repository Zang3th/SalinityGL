#include "Model.hpp"

namespace Engine
{
    // ----- Private -----

    void Model::InitGpuStorage(const Mesh* mesh)
    {
        //Create and bind vao
        _vao = MakeScope<VertexArray>();
        _vao->Bind();

        //Create vbo's, send it data and configure vao
        _vboVert = MakeScope<VertexBuffer>(&mesh->vertices[0], mesh->vertices.size() * sizeof(glm::vec3), GL_STATIC_DRAW);
        _vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

        _vboTex = MakeScope<VertexBuffer>(&mesh->texCoords[0], mesh->texCoords.size() * sizeof(glm::vec2), GL_STATIC_DRAW);
        _vao->DefineAttributes(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

        _vboNorm = MakeScope<VertexBuffer>(&mesh->normals[0], mesh->normals.size() * sizeof(glm::vec3), GL_STATIC_DRAW);
        _vao->DefineAttributes(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

        _vboTang = MakeScope<VertexBuffer>(&mesh->tangents[0], mesh->tangents.size() * sizeof(glm::vec3), GL_STATIC_DRAW);
        _vao->DefineAttributes(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

        //Create ibo
        _ibo = MakeScope<IndexBuffer>(&mesh->indices[0], mesh->indices.size() * sizeof(uint32));

        //Unbind vao
        _vao->Unbind();
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

    Model::Model(const Mesh* mesh)
        :   _model(glm::mat4(1.0f)),
            _position(0.0f),
            _textures(mesh->textures),
            _verticeCount(mesh->indices.size()),
            _gotNormalMap(mesh->gotNormalMap),
            _rotationX(0.0f), _rotationY(0.0f), _rotationZ(0.0f),
            _size(1.0f)
    {
        InitGpuStorage(mesh);
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

    void Model::BindBuffers() const
    {
        _vao->Bind();
        _ibo->Bind();
        _vboVert->Bind();
        _vboTex->Bind();
        _vboNorm->Bind();
        _vboTang->Bind();
    }

    void Model::UnbindBuffers() const
    {
        _vboTang->Unbind();
        _vboNorm->Unbind();
        _vboTex->Unbind();
        _vboVert->Unbind();
        _ibo->Unbind();
        _vao->Unbind();
    }

    void Model::ChangePosition(const glm::vec3& position)
    {
        _position += position;
        SetModelMatrix();
    }

    void Model::ChangeRotation(const glm::vec3& rotation)
    {
        _rotationX += rotation.x;
        _rotationY += rotation.y;
        _rotationZ += rotation.z;
        SetModelMatrix();
    }

    void Model::ChangeSize(const float size)
    {
        _size *= size;
        SetModelMatrix();
    }

    void Model::AddTexture(Texture* texture)
    {
        _textures.push_back(texture);
    }

    void Model::SetTextureInSlot(Texture* texture, uint32_t slot)
    {
        if(!_textures.empty())
            _textures.insert(_textures.begin() + slot, texture);
    }

    [[nodiscard]] const std::vector<Texture*>* Model::GetTextures() const
    {
        return &_textures;
    }
}