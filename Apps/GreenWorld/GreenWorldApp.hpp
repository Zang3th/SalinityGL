#pragma once

#include "Engine.hpp"
#include "GreenWorldInterface.hpp"

namespace GW
{
    class GreenWorldApp final : public Engine::App
    {
        private:
            Engine::SceneRenderer*                  _sceneRenderer  = nullptr;
            Engine::ShadowRenderer*                 _shadowRenderer = nullptr;
            Engine::WaterRenderer*                  _waterRenderer  = nullptr;
            Engine::SpriteRenderer*                 _spriteRenderer = nullptr;
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