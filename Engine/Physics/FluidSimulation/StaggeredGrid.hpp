#pragma once

#include "GlobalParams.hpp"

namespace Engine
{
    #define AT(x, y) ((x) * LiquefiedParams::SIMULATION_HEIGHT + (y))
    #define SWAP(p0, p1) {float *tmp=p0;p0=p1;p1=tmp;}

    struct StaggeredGrid
    {
        private:
            //Horizontal velocity (u-component) is sampled at the centers of the vertical cell faces.
            float _u[LiquefiedParams::SIMULATION_NUM_CELLS]     = {0.0f};
            float _u_tmp[LiquefiedParams::SIMULATION_NUM_CELLS] = {0.0f};

            //Vertical velocity (v-component) is sampled at the centers of the horizontal cell faces.
            float _v[LiquefiedParams::SIMULATION_NUM_CELLS]     = {0.0f};
            float _v_tmp[LiquefiedParams::SIMULATION_NUM_CELLS] = {0.0f};

            //b-component set to 0.0 for border cells.
            float _b[LiquefiedParams::SIMULATION_NUM_CELLS]     = {0.0f};

            //Density value for visualization of the flow.
            float _d[LiquefiedParams::SIMULATION_NUM_CELLS]     = {0.0f};
            float _d_tmp[LiquefiedParams::SIMULATION_NUM_CELLS] = {0.0f};

        public:
            const uint32 width  = LiquefiedParams::SIMULATION_WIDTH;
            const uint32 height = LiquefiedParams::SIMULATION_HEIGHT;
            const float  h      = 1.0f / (float)height; //Cell height
            const float  h2     = h * 0.5f;

            //Public accessors
            float* u     = &_u[0];
            float* u_tmp = &_u_tmp[0];
            float* v     = &_v[0];
            float* v_tmp = &_v_tmp[0];
            float* b     = &_b[0];
            float* d     = &_d[0];
            float* d_tmp = &_d_tmp[0];

            [[nodiscard]] inline float u_Avg(const uint32 x, const uint32 y) const
            {
                return (u[AT(x,y)] + u[AT(x+1,y)] + u[AT(x,y-1)] + u[AT(x+1,y-1)]) * 0.25f;
            }

            [[nodiscard]] inline float v_Avg(const uint32 x, const uint32 y) const
            {
                return (v[AT(x,y)] + v[AT(x-1,y)] + v[AT(x,y+1)] + v[AT(x-1,y+1)]) * 0.25f;
            }

            /* Samples the staggered grid with odd/floating point numbers and interpolates *
             * depending on the position between grid points.                              */
            [[nodiscard]] inline float SampleInterpolated(const float x, const float y, const float dx, const float dy, const float* f) const
            {
                const float numX = (float)width;
                const float numY = (float)height;

                //Check bounds
                float x_new = std::max(std::min(x, (numX * h)), h);
                float y_new = std::max(std::min(y, (numY * h)), h);

                //Calculate x-positions (x0, x1)
                float x0 = std::min(std::floor((x_new-dx)*numY), numX-1);
                x0 = std::max(x0, 0.0f); //Just to be safe
                const float x1 = std::min(x0+1, numX-1);

                //Calculate y-positions (y0, y1)
                float y0 = std::min(std::floor((y_new-dy)*numY), numY-1);
                y0 = std::max(y0, 0.0f); //Just to be safe
                const float y1 = std::min(y0+1, numY-1);

                //Calculate weight value for interpolation
                const float tx = ((x_new-dx) - x0*h) * numY;
                const float ty = ((y_new-dy) - y0*h) * numY;
                const float sx = 1.0f - tx;
                const float sy = 1.0f - ty;

                //Sample grid on calculated positions and apply interpolation factor
                const float val = sx*sy * f[AT((uint32)x0, (uint32)y0)] +
                                  tx*sy * f[AT((uint32)x1, (uint32)y0)] +
                                  tx*ty * f[AT((uint32)x1, (uint32)y1)] +
                                  sx*ty * f[AT((uint32)x0, (uint32)y1)];

                return val;
            }
    };
}
