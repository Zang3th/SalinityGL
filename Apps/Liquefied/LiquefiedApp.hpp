#pragma once

#include "Engine.hpp"
#include "LiquefiedInterface.hpp"

namespace Liq
{
    class LiquefiedApp final : public Engine::App
    {
        private:
            Engine::GridRenderer*                 _gridRenderer = nullptr;
            Engine::Scope<LiquefiedInterface>     _interface;
            Engine::Scope<Engine::FluidSimulator> _fluidSimulator;
            Engine::Scope<Engine::Timer>          _physicsTimer, _inputTimer;

            void           LoadResources()  override;
            void           AddBorderCells() const;
            Engine::uint32 InitModules()    override;
            void           UpdateTimer()    const;
            void           VisualizeSmoke() const;

        public:
            LiquefiedApp();
            ~LiquefiedApp()     override;
            void Update()       override;
            void ProcessInput() override;
    };
}