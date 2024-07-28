#pragma once

#include "Window.hpp"
#include "GlobalParams.hpp"
#include "StaggeredGrid.hpp"
#include "Profiler.hpp"

namespace Engine
{
    #define GRAVITY   PhysicsParams::GRAVITY
    #define ITER      LiquefiedParams::GAUSS_SEIDEL_ITERATIONS
    #define OVERRELAX LiquefiedParams::GAUSS_SEIDEL_OVERRELAXATION

    class FluidSimulator
    {
        private:
            StaggeredGrid _grid;

            void Init() const;
            void AddForces(float dt) const;
            void Project() const;
            void Extrapolate() const;
            void AdvectVelocity(float dt);
            void AdvectSmoke(float dt);
            void MonitorCFLStability(float dt, float value) const;
            void ForwardEuler(float dt, float pos, float h, float h2, float vel, float* prev_pos);

        public:
            FluidSimulator();
            ~FluidSimulator() = default;
            void TimeStep(float dt);
            void Reset() const;
            void AddBorderCell(uint32 x, uint32 y) const;
            void AddHorizonalTurbine(uint32 x, uint32 y, float power, float dt) const;

            [[nodiscard]] float GetDensity(uint32 x, uint32 y) const;
            [[nodiscard]] float GetBorder(uint32 x, uint32 y) const;
    };
}
