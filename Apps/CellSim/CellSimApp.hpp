#pragma once

#include "Engine.hpp"
#include "CellSimInterface.hpp"

namespace CS
{
    class CellSimApp final : public Engine::App
    {
        private:
            Engine::Scope<Engine::Timer>       _physicsTimer;
            Engine::uint8                      _tickCounter         = 0;
            bool                               _calcPhysicsThisTurn = true;
            Engine::SceneRenderer*             _sceneRenderer       = nullptr;
            Engine::Scope<Engine::CellManager> _cellManager;
            Engine::Scope<CellSimInterface>    _interface;

            void LoadResources() override;
            void AddObjects();
            void AddCellWorld();
            void HandleCellSpawn();
            void HandleCellKill();

        public:
            CellSimApp();
            ~CellSimApp() override;
            void Update() override;

            //Not needed in this app because we only use a general 3D camera controller to handle input.
            void ProcessInput() override {}

            [[nodiscard]] bool Init() override;
    };
}
