#pragma once

#include "Engine.hpp"
#include "GreenWorldInterface.hpp"

#include <vector>
#include <array>

namespace GreenWorld
{
    class App
    {
        private:
            Engine::Scope<Engine::Audio>            _audio;
            Engine::Scope<Engine::ShadowRenderer>   _shadowRenderer;
            Engine::Scope<Engine::Cubemap>          _cubemap;
            Engine::Scope<Engine::Sprite>           _shadowSprite, _reflectSprite, _refractSprite, _refractDepthSprite;
            Engine::Scope<Engine::WaterRenderer>    _waterRenderer;
            Engine::Scope<Engine::ParticleRenderer> _smokeRenderer;
            Engine::Scope<Interface>                _interface;

            const glm::vec3                         _lightPosition = glm::vec3(150.0f, 100.0f, -30.0f);
            const glm::vec3                         _lightColor    = glm::vec3(1.0f, 1.0f, 1.0f);
            const float                             _nearPlane     = 2.0f;
            const float                             _farPlane      = 300.0f;
            const float                             _waveSpeed     = 0.025f;
                  float                             _moveFactor    = 0.0f;
    
            void LoadResources();
            void InitModules();
            void CreateModels();
            void CreateParticles();
            void CreateCubemap();
            void CreateSprites();

        public:
            App();
            ~App();
            bool IsRunning();
            void Update();
    };
}