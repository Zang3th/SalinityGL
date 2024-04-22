#pragma once

#include "Window.hpp"
#include "GlobalParams.hpp"

#define num_X LiquiefiedParams::SIMULATION_WIDTH
#define num_Y LiquiefiedParams::SIMULATION_HEIGHT

namespace Engine
{
    class FluidSimulator
    {
        private:
            float _gravity = -9.81f;

            //Horizontal u-component is sampled at the centers of the vertical cell faces.
            float _u_staggered[LiquiefiedParams::LIQUID_NUM_CELLS] = {0.0f};

            //Vertical v-component is sampled at the centers of the horizontal cell faces.
            float _v_staggered[LiquiefiedParams::LIQUID_NUM_CELLS] = {0.0f};

            //s-component gets set to 0 for border cells.
            float _s_staggered[LiquiefiedParams::LIQUID_NUM_CELLS] = {0.0f};

            float _smokeField[LiquiefiedParams::LIQUID_NUM_CELLS]  = {0.0f};

            void AddForces(float dt);
            void Project(float dt);
            void AdvectVelocity(float dt);
            void AdvectSmoke(float dt);
            void Init();

        public:
            FluidSimulator();
            ~FluidSimulator();
            void TimeStep();
            float* GetSmokeField();
    };
}