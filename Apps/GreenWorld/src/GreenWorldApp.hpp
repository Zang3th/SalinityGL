#pragma once

#include "Engine.hpp"
#include "GreenWorldInterface.hpp"

namespace GW
{
    class GreenWorldApp final : public Engine::App
    {
        private:
            const float                             _nearPlane   = 2.0f;
            const float                             _farPlane    = 300.0f;
            const glm::vec3                         _lightPos    = glm::vec3(150.0f, 100.0f, -30.0f);
            const glm::vec3                         _lightTarget = glm::vec3(64.0f, 0.0f, 64.0f);
            const glm::vec3                         _lightCol    = glm::vec3(1.0f, 1.0f, 1.0f);

            Engine::SceneRenderer*                  _sceneRenderer  = nullptr;
            Engine::ShadowRenderer*                 _shadowRenderer = nullptr;
            Engine::SpriteRenderer*                 _spriteRenderer = nullptr;
            Engine::WaterRenderer*                  _waterRenderer  = nullptr;
            Engine::Scope<GreenWorldInterface>      _interface;
            Engine::Scope<Engine::Audio>            _audio;
    
            void           LoadResources() final;
            Engine::uint32 InitModules()   final;
            void           AddObjects();
            void           AddSprites();

        public:
            GreenWorldApp();
            ~GreenWorldApp() final;
            void Update() final;
    };
}