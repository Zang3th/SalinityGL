#pragma once

#include "Engine.hpp"
#include "CellSimInterface.hpp"

namespace CS
{
    class CellSimApp final : public Engine::App
    {
        private:
            double                             _timeElapsed         = 0.0;
            Engine::uint8                      _tickCounter         = 0;
            bool                               _calcPhysicsThisTurn = true;
            Engine::SceneRenderer*             _sceneRenderer       = nullptr;
            Engine::Scope<Engine::CellManager> _cellManager;
            Engine::Scope<CellSimInterface>    _interface;

            void           LoadResources() final;
            Engine::uint32 InitModules()   final;
            void           AddObjects();
            void           AddCellWorld();
            void           HandleCellSpawn();

        public:
            CellSimApp();
            ~CellSimApp() final;
            void Update() final;
    };
}