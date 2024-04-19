#include "PixelRenderer.hpp"

#include <ResourceManager.hpp>

namespace Engine
{
    // ----- Private -----

    PixelRenderer::PixelRenderer(const std::string& bgTexture, const std::string& shader)
        : _canvasSprite(ResourceManager::GetTexture(bgTexture),
                        ResourceManager::GetShader(shader),
                        COLOR_WHITE,
                        glm::vec2((float)WindowParams::WIDTH, (float)WindowParams::HEIGHT))
    {
        Logger::Info("Created", "Renderer", __func__);
    }

    // ----- Public -----

    void PixelRenderer::Flush(Renderer* renderer)
    {
        GLRenderSettings::DisableCulling();

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
}
