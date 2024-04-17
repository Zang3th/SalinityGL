#pragma once

#include "Engine.hpp"
#include "LiquefiedInterface.hpp"

namespace Liq
{
    class LiquefiedApp final : public Engine::App
    {
        private:
            Engine::Scope<LiquefiedInterface> _interface;

            void           LoadResources() final;
            Engine::uint32 InitModules()   final;

        public:
            LiquefiedApp();
            ~LiquefiedApp() final;
            void Update()   final;
    };
}