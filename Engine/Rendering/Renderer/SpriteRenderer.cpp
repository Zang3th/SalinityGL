#include "SpriteRenderer.hpp"

namespace Engine
{
    // ----- Public -----

    SpriteRenderer::SpriteRenderer()
    {
        Logger::Info("Created", "Renderer", __func__);
    }

    void SpriteRenderer::Flush(Renderer* renderer)
    {
        GLRenderSettings::DisableWireframe();
        GLRenderSettings::DisableCulling();

        //Render sprites
        for(const auto& sprite : _spriteStorage)
        {
            RenderStatistics::drawnVertices += sprite->Draw();
            RenderStatistics::drawCalls++;
        }

        //Increase render pass counter
        RenderStatistics::spritePasses++;

        GLRenderSettings::EnableCulling();
    }

    void SpriteRenderer::AddSprite(const glm::vec2& size, const glm::vec2& pos, Texture* texture, Shader* shader)
    {
        auto *sprite = new Sprite(texture, shader, glm::vec3(1.0f), size);
        sprite->ChangePosition(pos);
        _spriteStorage.push_back(sprite);
    }
}
