#pragma once

#include "Engine.hpp"
#include "CellSimInterface.hpp"

namespace CS
{
    class CellSimApp final : public Engine::App
    {
        private:
            double                             _timeElapsed    = 0.0;
            Engine::uint8                      _tickCounter    = 0;
            Engine::SceneRenderer*             _sceneRenderer  = nullptr;
            Engine::ShadowRenderer*            _shadowRenderer = nullptr;
            Engine::SpriteRenderer*            _spriteRenderer = nullptr;
            Engine::Scope<Engine::CellManager> _cellManager;
            Engine::Scope<CellSimInterface>    _interface;

            void           LoadResources() final;
            Engine::uint32 InitModules()   final;
            void           AddObjects();
            void           AddSprites();
            void           HandleCellSpawn();

        public:
            CellSimApp();
            ~CellSimApp() final;
            void Update() final;
    };
}