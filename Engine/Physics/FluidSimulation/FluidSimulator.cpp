#include "FluidSimulator.hpp"

namespace Engine
{
    // ----- Private -----

    /* Applies forces to the liquid - just gravity in this case. */
    void FluidSimulator::AddForces(const float dt)
    {
        for(uint32 x = 0; x < _grid.width; x++)
        {
            for(uint32 y = 0; y < _grid.height; y++)
            {
                //Check for border cell
                if(_grid.s_At(x, y) != 0.0f)
                {
                    _grid.v_At(x, y) += GRAVITY * dt;
                }
            }
        }
    }

    /* Projection calculates and applies the right amount of pressure to make the       *
     * velocity field divergence-free and also enforces solid wall boundary conditions. */
    void FluidSimulator::Project(const float dt)
    {
        //Simple solution using Gauss-Seidel
        for(uint32 it = 0; it < ITER; it++)
        {
            for(uint32 x = 1; x < _grid.width-1; x++)
            {
                for(uint32 y = 1; y < _grid.height-1; y++)
                {
                    //Skip border cells
                    if(_grid.s_At(x, y) == 0)
                    {
                        continue;
                    }

                    //Get the amount of fluid that is entering or leaving the cell
                    float divergence = _grid.u_At(x+1, y) - _grid.u_At(x, y) +
                                       _grid.v_At(x, y+1) - _grid.v_At(x, y);

                    //Apply overrelaxation to speed up convergence
                    divergence *= OVERRELAX;

                    //Get the amount of border cells in the area
                    float rightNeighbor = _grid.s_At(x+1, y);
                    float leftNeighbor  = _grid.s_At(x-1, y);
                    float upperNeigbor  = _grid.s_At(x, y+1);
                    float lowerNeighbor = _grid.s_At(x, y-1);

                    // Sum them up to later divide the divergence by the correct amount
                    float s_sum = rightNeighbor + leftNeighbor + upperNeigbor + lowerNeighbor;

                    //Push all velocities out by the same amout to force incompressibility
                    _grid.u_At(x, y)   += divergence * (leftNeighbor / s_sum);
                    _grid.u_At(x+1, y) -= divergence * (rightNeighbor / s_sum);
                    _grid.v_At(x, y)   += divergence * (lowerNeighbor / s_sum);
                    _grid.v_At(x, y+1) -= divergence * (upperNeigbor / s_sum);
                }
            }
        }
    }

    /* Advection moves the quantity, or molecules, or particles, along the velocity field. */
    void FluidSimulator::AdvectVelocity(const float dt)
    {

    }

    /* To visualize the flow, we store a density value at the center of each cell  *
     * and advect it like the velocity field.                                      */
    void FluidSimulator::AdvectSmoke(const float dt)
    {

    }

    void FluidSimulator::Init()
    {
        //Initialize all cells to fluid cells
        for(uint32 x = 0; x < _grid.width; x++)
        {
            for(uint32 y = 0; y < _grid.height; y++)
            {
                float cellValue = 1.0f;
                _grid.s_At(x, y) = cellValue;
            }
        }
    }

    // ----- Public -----

    FluidSimulator::FluidSimulator()
    {
        Init();
    }

    void FluidSimulator::TimeStep()
    {
        float dt = (float)Window::GetDeltaTime();
        AddForces(dt);
        Project(dt);
        AdvectVelocity(dt);
        AdvectSmoke(dt);
    }

    StaggeredGrid* FluidSimulator::GetGrid()
    {
        return &_grid;
    }
}