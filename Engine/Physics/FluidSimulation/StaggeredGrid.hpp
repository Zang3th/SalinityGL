#pragma once

#include "GlobalParams.hpp"

#include <cstring>

namespace Engine
{
    struct StaggeredGrid
    {
        private:
            //Horizontal velocity (u-component) is sampled at the centers of the vertical cell faces.
            float _u[LiquiefiedParams::LIQUID_NUM_CELLS]      = {0.0f};
            float _u_temp[LiquiefiedParams::LIQUID_NUM_CELLS] = {0.0f};

            //Vertical velocity (v-component) is sampled at the centers of the horizontal cell faces.
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

            //ToDo: float* u = &_u[0];

            //Spatial discretization
            const float dx = 1.0f;

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

            inline float u_Avg_At(const uint32 x, const uint32 y)
            {
                return (u_At(x,y) + u_At(x+1,y) + u_At(x,y-1) + u_At(x+1,y-1)) * 0.25f;
            }

            inline float v_Avg_At(const uint32 x, const uint32 y)
            {
                return (v_At(x,y) + v_At(x-1,y) + v_At(x,y+1) + v_At(x-1,y+1)) * 0.25f;
            }
    };
}