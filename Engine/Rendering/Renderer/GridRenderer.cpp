#include "GridRenderer.hpp"

namespace Engine
{
    // ----- Private -----

    GridRenderer::GridRenderer
    (
        const uint32 width,
        const uint32 height,
        const uint32 quadSize,
        const std::string& shader
    )
        :   _gridWidth(width), _gridHeight(height), _quadSize(quadSize), _quadAmountTotal(_gridWidth * _gridHeight),
            _defaultColor(glm::vec3(1.0f, 1.0f, 1.0f)), _shader(ResourceManager::GetShader(shader)),
            _orthoProj(glm::ortho(0.0f, (float)WindowParams::WIDTH, 0.0f, (float)WindowParams::HEIGHT, -1.0f, 1.0f)),
            _model(glm::scale(glm::mat4(1.0f), glm::vec3(glm::vec2((float)_quadSize), 0.0f)))
    {
        Logger::Info("Created", "Renderer", __func__);
        InitGpuStorage();
    }

    void GridRenderer::InitGpuStorage()
    {
        //Create vertice data
        static constexpr float vertices[] =
        {
            0.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
        };

        //Create and bind vao
        _vao = MakeScope<VertexArray>();
        _vao->Bind();

        //Create vbo's, send it data and configure vao
        _vboVert = MakeScope<VertexBuffer>(vertices, sizeof(vertices), GL_STATIC_DRAW);
        _vao->DefineAttributes(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

        //Unbind everything
        _vao->Unbind();
        _vboVert->Unbind();
    }

    void UpdateGpuStorage()
    {
        //...
    }

    // ----- Public -----

    void GridRenderer::Flush(Renderer* renderer)
    {
        //Check for Wireframe-Mode
        if(UIParams::wireframeRendering)
            GLRenderSettings::EnableWireframe();
        else
            GLRenderSettings::DisableWireframe();

        //Bind shader
        _shader->Bind();

        //Bind vao and vbo
        _vao->Bind();
        _vboVert->Bind();

        //Set uniforms
        _shader->SetUniformVec3f("color", _defaultColor);
        _shader->SetUniformMat4f("projection", _orthoProj);
        _shader->SetUniformMat4f("model", _model);

        //Render quad
        GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, _quadAmountTotal));

        //Unbind vao and vbo
        _vboVert->Unbind();
        _vao->Unbind();

        //Unbind shader
        _shader->Unbind();

        //ToDo: Update render stats
    }

    void GridRenderer::Set(const uint32 x, const uint32 y, const glm::vec3& color) const
    {
        //...
    }

    void GridRenderer::Reset(const uint32 x, const uint32 y) const
    {
        //...
    }

    void GridRenderer::SetScreen(const glm::vec3& color) const
    {
        //...
    }

    void GridRenderer::ClearScreen() const
    {
        //...
    }
}