#pragma once

#include "Engine.hpp"
#include "LiquefiedInterface.hpp"

namespace Liq
{
    class LiquefiedApp final : public Engine::App
    {
        private:
            Engine::PixelRenderer*            _pixelRenderer = nullptr;
            Engine::Scope<LiquefiedInterface> _interface;

            void           LoadResources() override;
            Engine::uint32 InitModules()   override;

        public:
            LiquefiedApp();
            ~LiquefiedApp() override;
            void Update()   override;
    };
}