#pragma once

#include "Engine.hpp"
#include "ParticleSimInterface.hpp"

namespace ParticleSim
{
    class App
    {
        private:
            Engine::Scope<Interface>                _interface;

            void LoadResources();
            void InitModules();

        public:
            App();
            ~App();
            bool IsRunning();
            void Update();
    };
}