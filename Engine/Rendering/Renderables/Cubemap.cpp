#include "Cubemap.hpp"

namespace Engine
{
    // ----- Private -----

    void Cubemap::InitGpuStorage()
    {
        //Create and bind vao
        _vao = MakeScope<VertexArray>();
        _vao->Bind();

        //Create vbo, send it data and configure vao
        _vboVert = MakeScope<VertexBuffer>(CUBE_VERTICES, sizeof(CUBE_VERTICES), GL_STATIC_DRAW);
        _vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        //Unbind vao
        _vao->Unbind();
    }

    // ----- Public -----

    Cubemap::Cubemap(const std::array<const char*, 6>& faces, Shader* shader)
        :   _verticeCount(36), _shader(shader), _cubemapTexture(MakeScope<CubemapTexture>(faces))
    {
        InitGpuStorage();
    }

    uint32 Cubemap::Draw(const glm::mat4& projMatrix, const glm::mat4& viewMatrix) const
    {
        //Remove translation section
        glm::mat4 view = glm::mat4(glm::mat3(viewMatrix));

        //Bind shader
        _shader->Bind();

        //Bind texture
        _cubemapTexture->Bind();

        //Bind vao and vbo
        _vao->Bind();
        _vboVert->Bind();

        //Set uniforms
        _shader->SetUniformMat4f("view", view);
        _shader->SetUniformMat4f("projection", projMatrix);
        _shader->SetUniform1i("textureSampler", 0);

        //Render
        GLCall(glDrawArrays(GL_TRIANGLES, 0, _verticeCount))

        //Unbind vao and vbo
        _vboVert->Unbind();
        _vao->Unbind();

        //Unbind texture
        _cubemapTexture->Unbind();

        //Unbind shader
        _shader->Unbind();

        //Return rendered vertices
        return _verticeCount;
    }
}