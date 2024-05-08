#pragma once

#include "GlobalParams.hpp"

namespace Engine
{
    #define AT(x, y) ((x) * LiquiefiedParams::SIMULATION_HEIGHT + (y))
    #define SWAP(p0, p1) {float *tmp=p0;p0=p1;p1=tmp;}

    struct StaggeredGrid
    {
        private:
            //Horizontal velocity (u-component) is sampled at the centers of the vertical cell faces.
            float _u[LiquiefiedParams::LIQUID_NUM_CELLS]     = {0.0f};
            float _u_tmp[LiquiefiedParams::LIQUID_NUM_CELLS] = {0.0f};

            //Vertical velocity (v-component) is sampled at the centers of the horizontal cell faces.
            float _v[LiquiefiedParams::LIQUID_NUM_CELLS]     = {0.0f};
            float _v_tmp[LiquiefiedParams::LIQUID_NUM_CELLS] = {0.0f};

            //b-component set to 0.0 for border cells.
            float _b[LiquiefiedParams::LIQUID_NUM_CELLS]     = {0.0f};

            //Density value for visualization of the flow.
            float _d[LiquiefiedParams::LIQUID_NUM_CELLS]     = {0.0f};
            float _d_tmp[LiquiefiedParams::LIQUID_NUM_CELLS] = {0.0f};

        public:
            const uint32 width  = LiquiefiedParams::SIMULATION_WIDTH;
            const uint32 height = LiquiefiedParams::SIMULATION_HEIGHT;

            //Public accessors
            float* u     = &_u[0];
            float* u_tmp = &_u_tmp[0];
            float* v     = &_v[0];
            float* v_tmp = &_v_tmp[0];
            float* b     = &_b[0];
            float* d     = &_d[0];
            float* d_tmp = &_d_tmp[0];

            //Spatial discretization
            const float dx = 1.0f;

            [[nodiscard]] inline float u_Avg(const uint32 x, const uint32 y) const
            {
                return (u[AT(x,y)] + u[AT(x+1,y)] + u[AT(x,y-1)] + u[AT(x+1,y-1)]) * 0.25f;
            }

            [[nodiscard]] inline float v_Avg(const uint32 x, const uint32 y) const
            {
                return (v[AT(x,y)] + v[AT(x-1,y)] + v[AT(x,y+1)] + v[AT(x-1,y+1)]) * 0.25f;
            }
    };
}