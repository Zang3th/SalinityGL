#include "PixelRenderer.hpp"

#include <ResourceManager.hpp>

namespace Engine
{
    // ----- Private -----

    PixelRenderer::PixelRenderer(const std::string& bgTexture, const std::string& shader)
        :   _width(ResourceManager::GetTexture(bgTexture)->GetWidth()),
            _height(ResourceManager::GetTexture(bgTexture)->GetHeight()),
            _canvasSprite(ResourceManager::GetTexture(bgTexture),
                        ResourceManager::GetShader(shader),
                        COLOR_WHITE,
                        glm::vec2(_width, _height))
    {
        Logger::Info("Created", "Renderer", __func__);
    }

    // ----- Public -----

    void PixelRenderer::Flush(Renderer* renderer)
    {
        GLRenderSettings::DisableCulling();

        //Commit texture changes
        _canvasSprite.GetTexture()->CommitModifications();

        //Render sprite
        RenderStatistics::drawnVertices += _canvasSprite.Draw();
        RenderStatistics::drawCalls++;

        //Increase render pass counter
        RenderStatistics::spritePasses++;

        GLRenderSettings::EnableCulling();
    }


    void PixelRenderer::Set(uint32 x, uint32 y, const glm::vec3& color) const
    {
        _canvasSprite.GetTexture()->ModifyTexture(x, y, color);
    }

    void PixelRenderer::Reset(uint32 x, uint32 y) const
    {
        _canvasSprite.GetTexture()->ResetTextureModification(x, y);
    }

    void PixelRenderer::SetScreen(const glm::vec3& color) const
    {
        for(uint32 x = 0; x < _width; x++)
        {
            for(uint32 y = 0; y < _height; y++)
            {
                Set(x, y, color);
            }
        }
    }

    void PixelRenderer::ClearScreen() const
    {
        for(uint32 x = 0; x < _width; x++)
        {
            for(uint32 y = 0; y < _height; y++)
            {
                Reset(x, y);
            }
        }
    }
}
