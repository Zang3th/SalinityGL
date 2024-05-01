#pragma once

#include "Window.hpp"
#include "GlobalParams.hpp"
#include "Monitoring.hpp"
#include "StaggeredGrid.hpp"

namespace Engine
{
    #define GRAVITY   PhysicsParams::GRAVITY
    #define ITER      LiquiefiedParams::GAUSS_SEIDEL_ITERATIONS
    #define OVERRELAX LiquiefiedParams::GAUSS_SEIDEL_OVERRELAXATION

    class FluidSimulator
    {
        private:
            StaggeredGrid _grid;

            [[nodiscard]] float ForwardEuler(float dt, float u, float q, float q_right, float q_left) const;
            float BackwardEuler();
            void  AddForces(float dt);
            void  CorrectForces();
            void  Project(float dt);
            void  AdvectVelocity(float dt);
            void  AdvectSmoke(float dt);
            void  Init();

        public:
            FluidSimulator();
            ~FluidSimulator() = default;
            void TimeStep();
            void Reset();
            void AddHorizonalTurbine(uint32 x, uint32 y, float power);
            StaggeredGrid* GetGrid();
    };
}