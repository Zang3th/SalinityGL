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
    
            void           LoadResources() override;
            Engine::uint32 InitModules()   override;
            void           AddObjects();
            void           AddSprites();

        public:
            GreenWorldApp();
            ~GreenWorldApp() override;
            void Update()    override;

            //Not needed in this app because we only use a general 3D camera controller to handle input.
            void ProcessInput() override {}
    };
}