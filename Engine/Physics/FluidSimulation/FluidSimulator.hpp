#pragma once

#include "Window.hpp"
#include "GlobalParams.hpp"

#define at(x, y) ((x) * Liquiedfied::Params::SIMULATION_HEIGHT + (y))

namespace Engine
{
    class FluidSimulator
    {
        private:
            float _gravity = -9.81f;
            float _velocityField[LiquiefiedParams::LIQUID_NUM_CELLS] = {0};
            float _smokeField[LiquiefiedParams::LIQUID_NUM_CELLS]    = {0};

            void AddForces(float dt);
            void Project(float dt);
            void AdvectVelocity(float dt);
            void AdvectSmoke(float dt);

        public:
            FluidSimulator();
            ~FluidSimulator();
            void TimeStep();
            float* GetSmokeField();
    };
}