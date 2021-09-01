#include "Cubemap.hpp"

namespace Core
{
    // ----- Private -----

    Ref<VertexArray> Cubemap::CreateVao()
    {
        float skyboxVertices[] =
        {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
        };

        //Create and bind vao
        Ref<VertexArray> vao = MakeRef<VertexArray>();
        vao->Bind();

        //Create vbo, send it data and configure vao
        VertexBuffer vbo(&skyboxVertices, sizeof(skyboxVertices));
        vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        //Unbind vao
        vao->Unbind();

        return vao;
    }

    // ----- Public -----

    Cubemap::Cubemap(const std::array<const char*, 6>& faces, Shader* shader)
        :   _vao(CreateVao()),
            _cubemapTexture(MakeScope<CubemapTexture>(faces)),
            _shader(shader),
            _verticeCount(36)
    {

    }

    uint32 Cubemap::Draw(const glm::mat4& projMatrix, const glm::mat4& viewMatrix) const
    {
        _shader->Bind();

        //Remove translation section
        glm::mat4 view = glm::mat4(glm::mat3(viewMatrix));

        //Set uniforms
        _shader->SetUniformMat4f("view", view);
        _shader->SetUniformMat4f("projection", projMatrix);
        _shader->SetUniform1i("textureSampler", 0);

        //Set texture
        _cubemapTexture->Bind();
        _vao->Bind();

        //Render
        GLCall(glDrawArrays(GL_TRIANGLES, 0, _verticeCount));

        _vao->Unbind();
        _cubemapTexture->Unbind();
        _shader->Unbind();

        //Return rendered vertices
        return _verticeCount;
    }
}