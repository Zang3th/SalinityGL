#include "GridRenderer.hpp"

namespace Engine
{
    // ----- Private -----

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

        //Init color storage
        for(uint32 i = 0; i < _quadAmountTotal; i++)
        {
            _colorStorage.emplace_back(_defaultColor, _defaultGradientFactor);
        }

        //Create and bind vao
        _vao = MakeScope<VertexArray>();
        _vao->Bind();

        //Create vbo's, send it data and configure vao
        _vboVert = MakeScope<VertexBuffer>(vertices, sizeof(vertices), GL_STATIC_DRAW);
        _vao->DefineAttributes(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
        _vboColor = MakeScope<VertexBuffer>(&_colorStorage[0], _colorStorage.size() * 4 * sizeof(float), GL_DYNAMIC_DRAW);
        _vao->DefineAttributes(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

        //Set attribute divisors for per instance data
        _vao->AttributeDivisor(1, 1);
        _vao->AttributeDivisor(2, 1);

        //Unbind everything
        _vao->Unbind();
        _vboVert->Unbind();
        _vboColor->Unbind();
    }

    // ----- Public -----

    GridRenderer::GridRenderer
    (
        const uint32 width,
        const uint32 height,
        const uint32 quadSize,
        const std::string& shader
    )
        :   _gridWidth(width), _gridHeight(height), _quadSize(quadSize), _quadAmountTotal(_gridWidth * _gridHeight),
            _defaultColor(glm::vec3(0.1f, 0.1f, 0.1f)), _defaultGradientFactor(0.000025f), _shader(ResourceManager::GetShader(shader)),
            _orthoProj(glm::ortho(0.0f, (float)WindowParams::WIDTH, 0.0f, (float)WindowParams::HEIGHT, -1.0f, 1.0f)),
            _model(glm::scale(glm::mat4(1.0f), glm::vec3(glm::vec2((float)_quadSize), 0.0f)))
    {
        Logger::Info("Created", "Renderer", __func__);
        _colorStorage.reserve(_quadAmountTotal);
        InitGpuStorage();
    }

    void GridRenderer::Flush(Renderer* renderer)
    {
        //Check for Wireframe-Mode
        if(UIParams::wireframeRendering)
        {
            GLRenderSettings::EnableWireframe();
        }
        else
        {
            GLRenderSettings::DisableWireframe();
        }

        //Bind shader
        _shader->Bind();

        //Bind vao and vbo's
        _vao->Bind();
        _vboVert->Bind();
        _vboColor->Bind();

        //Set uniforms
        _shader->SetUniformVec3f("color", _defaultColor);
        _shader->SetUniformMat4f("projection", _orthoProj);
        _shader->SetUniformMat4f("model", _model);

        //Render quad
        GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, _quadAmountTotal));

        //Unbind vao and vbo's
        _vboColor->Unbind();
        _vboVert->Unbind();
        _vao->Unbind();

        //Unbind shader
        _shader->Unbind();

        //Save stats
        RenderStatistics::drawnVertices += 6;
        RenderStatistics::drawCalls++;
    }

    void GridRenderer::UpdateGpuStorage() const
    {
        _vboColor->Bind();
        _vboColor->Update(&_colorStorage[0], _colorStorage.size() * 4 * sizeof(float));
        _vboColor->Unbind();
    }

    void GridRenderer::Set(const uint32 x, const uint32 y, const glm::vec3& color)
    {
        _colorStorage.at(x * _gridHeight + y) = glm::vec4(color, 0.0f);
    }

    void GridRenderer::Reset(const uint32 x, const uint32 y)
    {
        _colorStorage.at(x * _gridHeight + y) = glm::vec4(_defaultColor, _defaultGradientFactor);
    }

    void GridRenderer::SetConfigAsDefault()
    {
        _backupStorage = _colorStorage;
    }

    void GridRenderer::UploadDefaultConfig() const
    {
        _vboColor->Bind();
        _vboColor->Update(&_backupStorage[0], _backupStorage.size() * 4 * sizeof(float));
        _vboColor->Unbind();
    }
}
