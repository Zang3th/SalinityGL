#pragma once

#include "Renderer.hpp"
#include "glm.hpp"
#include "Sprite.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "GLRenderSettings.hpp"
#include "GlobalParams.hpp"

#include <vector>

namespace Engine
{
    class PixelRenderer final : public Renderer
    {
        friend class RenderManager;

        private:
            uint32 _width, _height;
            Sprite _canvasSprite;

            PixelRenderer(const std::string& bgTexture, const std::string& shader);

        public:
            void Flush(Renderer* renderer) override;
            void Set(uint32 x, uint32 y, const glm::vec3& color) const;
            void Reset(uint32 x, uint32 y) const;
            void SetScreen(const glm::vec3& color) const;
            void ClearScreen() const;
    };
}