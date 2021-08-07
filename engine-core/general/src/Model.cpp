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

    // ----- Public -----

    Model::Model(Texture* texture, Shader* shader, glm::vec3 color, Mesh* mesh)
        : _texture(texture), _shader(shader), _color(color), _model(glm::mat4(1.0f)), _verticeCount(0)
    {
        _vao = CreateVaoFromMesh(mesh);
    }

    Model::Model(Texture* texture, Shader* shader, glm::vec3 color, const std::string& filepath)
        : _texture(texture), _shader(shader), _color(color), _model(glm::mat4(1.0f)), _verticeCount(0)
    {
        Mesh mesh;
        Loader loader;
        loader.GLTFToMesh(filepath, &mesh);
        _vao = CreateVaoFromMesh(&mesh);
    }

    unsigned int Model::Draw(const glm::mat4& projMatrix, const glm::mat4& viewMatrix, const glm::vec3& camPos)
    {
        _shader->Bind();

        //Set uniforms
        _shader->SetUniformMat4f("view", viewMatrix);
        _shader->SetUniformMat4f("model", _model);
        _shader->SetUniformMat4f("projection", projMatrix);
        _shader->SetUniformVec3f("viewPos", camPos);
        _shader->SetUniformVec3f("color", _color);

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
}