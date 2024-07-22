#include "GridRenderer.hpp"
#include "Utility.hpp"

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

        //Init storage
        for(uint32 i = 0; i < _quadAmountTotal; i++)
        {
            _colorStorage.emplace_back(0.0f);
            _backupStorage.emplace_back(0.0f);
        }

        //Create and bind vao
        _vao = MakeScope<VertexArray>();
        _vao->Bind();

        //Create vbo's, send it data and configure vao
        _vboVert = MakeScope<VertexBuffer>(vertices, sizeof(vertices), GL_STATIC_DRAW);
        _vao->DefineAttributes(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
        _vboColor = MakeScope<VertexBuffer>(&_colorStorage[0], _colorStorage.size() * 3 * sizeof(float), GL_DYNAMIC_DRAW);
        _vao->DefineAttributes(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

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
            _shader(ResourceManager::GetShader(shader)), _defaultColor(0.0f),
            _orthoProj(glm::ortho(0.0f, (float)WindowParams::WIDTH, 0.0f, (float)WindowParams::HEIGHT, -1.0f, 1.0f)),
            _model(glm::scale(glm::mat4(1.0f), glm::vec3(glm::vec2((float)_quadSize), 0.0f)))
    {
        Logger::Info("Created", "Renderer", __func__);

        _colorStorage.reserve(_quadAmountTotal);
        _backupStorage.reserve(_quadAmountTotal);

        InitGpuStorage();
    }

    void GridRenderer::SetConfigAsDefault()
    {
        _backupStorage = _colorStorage;
    }

    void GridRenderer::SetDefaultColor(const glm::vec3& color)
    {
        _defaultColor = color;
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
        _vboColor->Update(&_colorStorage[0], _colorStorage.size() * 3 * sizeof(float));
        _vboColor->Unbind();
    }

    void GridRenderer::Set(const uint32 x, const uint32 y, const glm::vec3& color)
    {
        _colorStorage.at(x * _gridHeight + y) = glm::vec3(color);
    }

    void GridRenderer::SetArea(const glm::uvec2& pos, uint32 size, const glm::vec3& color)
    {
        for(uint32 x = pos.x; x < pos.x + size; x++)
        {
            for(uint32 y = pos.y; y < pos.y + size; y++)
            {
                Set(x, y, color);
            }
        }
    }

    void GridRenderer::Reset(const uint32 x, const uint32 y)
    {
        _colorStorage.at(x * _gridHeight + y) = _backupStorage.at(x * _gridHeight + y);
    }

    void GridRenderer::AddTextureBufferSubsampled(const std::string& texBuffer, const glm::uvec2& pos, const uint32 size)
    {
        auto* textureBuffer = ResourceManager::GetTextureBuffer(texBuffer);
        uint32 width  = textureBuffer->GetWidth();
        uint32 height = textureBuffer->GetHeight();

        if(width != height || width % size != 0 || height % size != 0)
        {
            Logger::Error("Failed", "Subsampling", "Dimensions or format unsupported");
            return;
        }

        //Set texture area to default color
        SetArea(pos, size, _defaultColor);

        //sampleAmount stands for the amount of pixels that needs to be sampled in one direction.
        //F.E. the original image is 512x512 and we want to reduce it to 8x8.
        //That results in 64x64 pixels that will get sampled for 1 pixel in the new image.
        //In our case these pixels are cells in the grid.
        uint32 sampleAmount = width / size;

        glm::uvec2 gridPos = pos;
        bool success = false;

        //Go over the image in sampleAmount steps
        for(uint32 x = 0; x < width; x += sampleAmount)
        {
            for(uint32 y = 0; y < height; y += sampleAmount)
            {
                glm::uvec3 subsampledColor = {0, 0, 0};
                success = textureBuffer->SubsampleArea(x, y, sampleAmount, &subsampledColor);

                if(success)
                {
                    Set(gridPos.x, gridPos.y, Utility::TransformVec3uTo3f(subsampledColor));
                }

                gridPos.y++;
            }

            gridPos.x++;
            gridPos.y = pos.y; //Reset y-position
        }
    }
}
