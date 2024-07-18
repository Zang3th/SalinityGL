#pragma once

#include "Renderer.hpp"
#include "glm.hpp"
#include "GLTexture.hpp"
#include "Shader.hpp"
#include "GLRenderSettings.hpp"
#include "Sprite.hpp"
#include "GlobalParams.hpp"

#include <vector>

namespace Engine
{
    class SpriteRenderer final : public Renderer
    {
        private:
            std::vector<Sprite*> _spriteStorage;

        public:
            SpriteRenderer();
            void Flush(Renderer* renderer) override;
            void AddSprite(const glm::vec2& size, const glm::vec2& pos, GLTexture* glTexture, Shader* shader);
    };
}
