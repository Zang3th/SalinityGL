#include "PixelRenderer.hpp"

namespace Engine
{
    // ----- Public -----

    PixelRenderer::PixelRenderer
    (
        const uint32 width,
        const uint32 height,
        const uint32 pxSize,
        const std::string& bgTexture,
        const std::string& shader
    )
        :   _width(width), _height(height), _pxSize(pxSize),
            _canvasSprite(ResourceManager::GetGLTexture(bgTexture),
                          ResourceManager::GetShader(shader),
                          COLOR_WHITE,
                          glm::vec2(_width * _pxSize, _height * _pxSize))
    {
        Logger::Info("Created", "Renderer", __func__);
    }

    void PixelRenderer::Flush(Renderer* renderer)
    {
        //Commit texture changes
        _canvasSprite.GetGLTexture()->UploadModifiedBuffer();

        //Render sprite
        RenderStatistics::drawnVertices += _canvasSprite.Draw();
        RenderStatistics::drawCalls++;

        //Increase render pass counter
        RenderStatistics::spritePasses++;
    }

    void PixelRenderer::Set(const uint32 x, const uint32 y, const PxColor& colorIn) const
    {
        const uint32 x_end   = x * _pxSize;
        const uint32 y_end   = y * _pxSize;
        const uint32 x_start = x_end - _pxSize;
        const uint32 y_start = y_end - _pxSize;

        SetArea(x_start, x_end, y_start, y_end, colorIn);
    }

    void PixelRenderer::SetArea(const uint32 x_start, const uint32 x_end, const uint32 y_start, const uint32 y_end, const PxColor& colorIn) const
    {
        for(uint32 x = x_start; x < x_end; x++)
        {
            for(uint32 y = y_start; y < y_end; y++)
            {
                //Modify texture buffer and break/quit if something goes wrong
                if(_canvasSprite.GetGLTexture()->GetTextureBuffer()->SetPxColor(x, y, colorIn) == false)
                {
                    break;
                }
            }
        }
    }

    void PixelRenderer::ResetArea(const uint32 x_start, const uint32 x_end, const uint32 y_start, const uint32 y_end) const
    {
        for(uint32 x = x_start; x < x_end; x++)
        {
            for(uint32 y = y_start; y < y_end; y++)
            {
                //Reset texture buffer back to the default values and break/quit if something goes wrong
                if(_canvasSprite.GetGLTexture()->GetTextureBuffer()->ResetPxColor(x, y))
                {
                    break;
                }
            }
        }
    }

    void PixelRenderer::SetScreen(const PxColor& colorIn) const
    {
        SetArea(0, _width * _pxSize, 0, _height * _pxSize, colorIn);
    }

    void PixelRenderer::ClearScreen() const
    {
        ResetArea(0, _width * _pxSize, 0, _height * _pxSize);
    }
}
