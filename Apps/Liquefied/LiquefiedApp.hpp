#pragma once

#include "Engine.hpp"
#include "LiquefiedInterface.hpp"

namespace Liq
{
    class LiquefiedApp final : public Engine::App
    {
        private:
            double                                _timeElapsed   = 0.0;
            Engine::GridRenderer*                 _gridRenderer  = nullptr;
            Engine::Scope<LiquefiedInterface>     _interface;
            Engine::Scope<Engine::FluidSimulator> _fluidSimulator;

            void           LoadResources()  override;
            void           AddBorderCells() const;
            Engine::uint32 InitModules()    override;
            void           VisualizeSmoke() const;

        public:
            LiquefiedApp();
            ~LiquefiedApp() override;
            void Update()   override;
    };
}