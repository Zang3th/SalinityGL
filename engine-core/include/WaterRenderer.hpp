#pragma once

#include "FrameBuffer.hpp"

namespace Core
{
    class WaterRenderer
    {
        private:
            Scope<FrameBuffer>  _reflectFBO, _refractFBO;

        public:
            WaterRenderer();
            void StartFrame();
            void EndFrame();
            [[nodiscard]] Texture* GetReflectTexture() const;
            [[nodiscard]] Texture* GetRefractTexture() const;
            [[nodiscard]] Texture* GetRefractDepthTexture() const;
    };
}