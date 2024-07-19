#pragma once

#include "Engine.hpp"
#include "LiquefiedInterface.hpp"

namespace Liq
{
    class LiquefiedApp final : public Engine::App
    {
        private:
            Engine::GridRenderer*                 _gridRenderer   = nullptr;
            Engine::SpriteRenderer*               _spriteRenderer = nullptr;
            Engine::Scope<LiquefiedInterface>     _interface;
            Engine::Scope<Engine::FluidSimulator> _fluidSimulator;
            Engine::Scope<Engine::Timer>          _physicsTimer, _inputTimer;
            const glm::uvec2  turbinePos    = {1, 50};
            const glm::uint32 turbineSize   = 8;
            const glm::uvec2  turbineOutlet =
            {
                turbinePos.x + turbineSize,
                turbinePos.y + turbineSize / 2
            };
            const glm::uvec2  obstaclePos   = {50, 50};
            const glm::uint32 obstacleSize  = 4;

            void LoadResources()               override;
            void AddBorderCells()              const;
            void AddTurbine()                  const;
            void AddObstacles()                const;
            void TurbinePushVelocity(float dt) const;
            void UpdateTimer()                 const;
            void RenderSmoke()                 const;

        public:
            LiquefiedApp();
            ~LiquefiedApp()           override;
            void Update()             override;
            void ProcessInput()       override;
            [[nodiscard]] bool Init() override;
    };
}
