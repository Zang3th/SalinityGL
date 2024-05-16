#pragma once

#include "Window.hpp"
#include "GlobalParams.hpp"
#include "StaggeredGrid.hpp"
#include "Profiler.hpp"

namespace Engine
{
    #define GRAVITY   PhysicsParams::GRAVITY
    #define ITER      LiquiefiedParams::GAUSS_SEIDEL_ITERATIONS
    #define OVERRELAX LiquiefiedParams::GAUSS_SEIDEL_OVERRELAXATION

    class FluidSimulator
    {
        private:
            StaggeredGrid _grid;

            void Init() const;
            void AddForces(float dt) const;
            void Project(float dt) const;
            void Extrapolate();
            void AdvectVelocity(float dt);
            void AdvectSmoke(float dt);
            [[nodiscard]] float ForwardEuler(float dt, float u, float q, float q_next, float q_prev) const;
            [[nodiscard]] float ModifiedForwardEuler(float dt, float vel, const float* q_values, uint32 q_startX, uint32 q_startY) const;
            [[nodiscard]] float BackwardEuler(float dt, float vel, const float* q_values, uint32 q_startX, uint32 q_startY) const;
            [[nodiscard]] float SemiLagrangian(float x, float y, float dx, float dy, const float* f) const;
            [[nodiscard]] uint32 NewtonRaphson(const float* values, uint32 startX, uint32 startY, uint32 maxIteration) const;

        public:
            FluidSimulator();
            ~FluidSimulator() = default;
            void TimeStep(float dt);
            void Reset() const;
            void AddBorderCell(uint32 x, uint32 y) const;
            void AddHorizonalTurbine(uint32 x, uint32 y, float power, float dt) const;
            [[nodiscard]] float GetDensity(uint32 x, uint32 y) const;
    };
}