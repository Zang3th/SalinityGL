#pragma once

#include "Engine.hpp"
#include "LiquefiedInterface.hpp"

namespace Liq
{
    class LiquefiedApp final : public Engine::App
    {
        private:
            Engine::GridRenderer*                 _gridRenderer   = nullptr;

            Engine::Scope<LiquefiedInterface>     _interface;
            Engine::Scope<Engine::FluidSimulator> _fluidSimulator;
            Engine::Scope<Engine::Timer>          _physicsTimer, _inputTimer;

            const glm::vec3   _defaultColor  = {0.5f, 0.5f, 0.5f};
            const glm::uvec2  _turbinePos    = {1, 35};
            const glm::uint32 _turbineSize   = 32;
            const glm::uvec2  _turbineOutlet =
            {
                _turbinePos.x + _turbineSize,
                _turbinePos.y + _turbineSize / 2
            };
            const glm::uvec2  _obstaclePos   = {70, 45};
            const glm::uint32 _obstacleSize  = 16;

            void LoadResources()               override;
            void AddBorderCells()              const;
            void AddTurbine()                  const;
            void AddObstacle()                 const;
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
