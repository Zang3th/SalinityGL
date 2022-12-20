#pragma once

#include "Engine.hpp"
#include "ParticleSimInterface.hpp"

namespace ParticleSim
{
    class App
    {
        private:
            Engine::Scope<Engine::Sprite>           _bgSprite;
            Engine::Scope<Interface>                _interface;

            void LoadResources();
            void InitModules();
            void CreateSprites();

        public:
            App();
            ~App();
            bool IsRunning();
            void Update();
    };
}