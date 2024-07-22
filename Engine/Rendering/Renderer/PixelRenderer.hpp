#pragma once

#include "Renderer.hpp"
#include "glm.hpp"
#include "Sprite.hpp"
#include "GLTexture.hpp"
#include "Shader.hpp"
#include "GLRenderSettings.hpp"
#include "GlobalParams.hpp"
#include "ResourceManager.hpp"

namespace Engine
{
    class PixelRenderer final : public Renderer
    {
        private:
            uint32 _width, _height, _pxSize;
            Sprite _canvasSprite;

        public:
            PixelRenderer
            (
                uint32             width,
                uint32             height,
                uint32             pxSize,
                const std::string& bgTexture,
                const std::string& shader
            );

            void Flush(Renderer* renderer) override;
            void Set(uint32 x, uint32 y, const PxColor& colorIn) const;
            void SetArea(uint32 x_start, uint32 x_end, uint32 y_start, uint32 y_end, const PxColor& colorIn) const;
            void ResetArea(uint32 x_start, uint32 x_end, uint32 y_start, uint32 y_end) const;
            void SetScreen(const PxColor& colorIn) const;
            void ClearScreen() const;
    };
}
