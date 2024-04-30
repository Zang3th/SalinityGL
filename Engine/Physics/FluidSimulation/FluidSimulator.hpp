#pragma once

#include "Window.hpp"
#include "GlobalParams.hpp"
#include "Monitor.hpp"

#include <cstring>

namespace Engine
{
    #define GRAVITY   PhysicsParams::GRAVITY
    #define ITER      LiquiefiedParams::GAUSS_SEIDEL_ITERATIONS
    #define OVERRELAX LiquiefiedParams::GAUSS_SEIDEL_OVERRELAXATION

    struct StaggeredGrid
    {
        private:
            //Horizontal u-component is sampled at the centers of the vertical cell faces.
            float _u[LiquiefiedParams::LIQUID_NUM_CELLS]      = {0.0f};
            float _u_temp[LiquiefiedParams::LIQUID_NUM_CELLS] = {0.0f};

            //Vertical is sampled at the centers of the horizontal cell faces.
            float _v[LiquiefiedParams::LIQUID_NUM_CELLS]      = {0.0f};
            float _v_temp[LiquiefiedParams::LIQUID_NUM_CELLS] = {0.0f};

            //s-comp set to 0 for border cells.
            float _s[LiquiefiedParams::LIQUID_NUM_CELLS]      = {0.0f};

            //For visualization of the flow.
            float _smoke[LiquiefiedParams::LIQUID_NUM_CELLS]      = {0.0f};
            float _smoke_temp[LiquiefiedParams::LIQUID_NUM_CELLS] = {0.0f};

        public:
            const uint32 width  = LiquiefiedParams::SIMULATION_WIDTH;
            const uint32 height = LiquiefiedParams::SIMULATION_HEIGHT;

            //Spatial discretization
            const float deltaX = 1.0f;
            const float deltaY = 1.0f;

            //Access to internal arrays
            inline float& u_At         (const uint32 x, const uint32 y) { return _u         [x * height + y]; }
            inline float& u_temp_At    (const uint32 x, const uint32 y) { return _u_temp    [x * height + y]; }
            inline float& v_At         (const uint32 x, const uint32 y) { return _v         [x * height + y]; }
            inline float& v_temp_At    (const uint32 x, const uint32 y) { return _v_temp    [x * height + y]; }
            inline float& s_At         (const uint32 x, const uint32 y) { return _s         [x * height + y]; }
            inline float& smoke_At     (const uint32 x, const uint32 y) { return _smoke     [x * height + y]; }
            inline float& smoke_temp_At(const uint32 x, const uint32 y) { return _smoke_temp[x * height + y]; }

            inline void UpdateGridVelocities()
            {
                //Copy temp arrays into u- and v-arrays
                std::memcpy(&_u[0], &_u_temp[0], LiquiefiedParams::LIQUID_NUM_CELLS);
                std::memcpy(&_v[0], &_v_temp[0], LiquiefiedParams::LIQUID_NUM_CELLS);
            }

            inline void UpdateGridSmoke(){ std::memcpy(&_smoke[0], &_smoke_temp[0], LiquiefiedParams::LIQUID_NUM_CELLS); }
    };

    class FluidSimulator
    {
        private:
            StaggeredGrid _grid;

            float ForwardEuler(float dt, float dx, float vel_field, float q, float q_right, float q_left);
            float BackwardEuler();

            void  AddForces(float dt);
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