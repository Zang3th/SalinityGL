#include "PixelRenderer.hpp"

namespace Engine
{
    // ----- Private -----

    PixelRenderer::PixelRenderer
    (
        const uint32 width,
        const uint32 height,
        const uint32 pxSize,
        const std::string& bgTexture,
        const std::string& shader
    )
        :   _width(width), _height(height), _pxSize(pxSize),
            _canvasSprite(ResourceManager::GetTexture(bgTexture),
                          ResourceManager::GetShader(shader),
                          COLOR_WHITE,
                          glm::vec2(_width * _pxSize, _height * _pxSize))
    {
        Logger::Info("Created", "Renderer", __func__);
    }

    // ----- Public -----

    void PixelRenderer::Flush(Renderer* renderer)
    {
        //Commit texture changes
        _canvasSprite.GetTexture()->CommitModifications();

        //Render sprite
        RenderStatistics::drawnVertices += _canvasSprite.Draw();
        RenderStatistics::drawCalls++;

        //Increase render pass counter
        RenderStatistics::spritePasses++;
    }

    void PixelRenderer::Set(const uint32 x, const uint32 y, const glm::vec3& color) const
    {
        const uint32 x_end   = x * _pxSize;
        const uint32 y_end   = y * _pxSize;
        const uint32 x_start = x_end - _pxSize;
        const uint32 y_start = y_end - _pxSize;

        SetArea(x_start, x_end, y_start, y_end, color);
    }

    void PixelRenderer::SetArea(const uint32 x_start, const uint32 x_end, const uint32 y_start, const uint32 y_end, const glm::vec3& color) const
    {
        for(uint32 x = x_start; x < x_end; x++)
        {
            for(uint32 y = y_start; y < y_end; y++)
            {
                _canvasSprite.GetTexture()->ModifyTexture(x, y, color);
            }
        }
    }

    void PixelRenderer::ResetArea(const uint32 x_start, const uint32 x_end, const uint32 y_start, const uint32 y_end) const
    {
        for(uint32 x = x_start; x < x_end; x++)
        {
            for(uint32 y = y_start; y < y_end; y++)
            {
                _canvasSprite.GetTexture()->ResetTextureModification(x, y);
            }
        }
    }

    void PixelRenderer::SetScreen(const glm::vec3& color) const
    {
        SetArea(0, _width * _pxSize, 0, _height * _pxSize, color);
    }

    void PixelRenderer::ClearScreen() const
    {
        ResetArea(0, _width * _pxSize, 0, _height * _pxSize);
    }
}
