#include "SpriteRenderer.hpp"

namespace Engine
{
    // ----- Private -----

    SpriteRenderer::SpriteRenderer()
        : _orthoProj(glm::ortho(0.0f, (float)WindowParams::WIDTH, 0.0f, (float)WindowParams::HEIGHT, -1.0f, 1.0f))
    {
        Logger::Info("Created", "Renderer",__func__);
    }

    // ----- Public -----

    void SpriteRenderer::Flush(Renderer* renderer)
    {
        GLRenderSettings::DisableWireframe();
        GLRenderSettings::DisableCulling();

        //Render sprites
        for(const auto& sprite : _spriteStorage)
        {
            RenderStatistics::drawnVertices += sprite->Draw(_orthoProj);
            RenderStatistics::drawCalls++;
        }

        //Increase render pass counter
        RenderStatistics::spritePasses++;

        GLRenderSettings::EnableCulling();
    }

    void SpriteRenderer::AddSprite(const glm::vec2& size, const glm::vec2& pos, Texture* texture, Shader* shader)
    {
        auto sprite = new Sprite(texture, shader, glm::vec3(1.0f));
        sprite->ChangeSize(size);
        sprite->ChangePosition(pos);
        _spriteStorage.push_back(sprite);
    }
}