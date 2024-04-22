#include "FluidSimulator.hpp"

namespace Engine
{
    // ----- Private -----

    /* Applies forces to the liquid - just gravity in our case. */
    void FluidSimulator::AddForces(const float dt)
    {
        for(uint32 x = 0; x < num_X; x++)
        {
            for(uint32 y = 0; y < num_Y; y++)
            {
                //Check for border cell
                if(_s_staggered[x * num_Y + y] != 0.0f)
                {
                    _v_staggered[x * num_Y + y] += _gravity * dt;
                }
            }
        }
    }

    /* Projection calculates and applies the right amount of pressure to make the       *
     * velocity field divergence-free and also enforces solid wall boundary conditions. */
    void FluidSimulator::Project(const float dt)
    {

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
        //Initialize border cells
        for(uint32 x = 0; x < num_X; x++)
        {
            for(uint32 y = 0; y < num_Y; y++)
            {
                float cellValue = 1.0f; //Fluid cell

                if((x == 0) || (y == 0) || (x == num_X - 1) || (y == num_Y - 1))
                {
                    cellValue = 0.0f; //Solid cell
                }

                _s_staggered[x * num_Y + y] = cellValue;
            }
        }
    }

    // ----- Public -----

    FluidSimulator::FluidSimulator()
    {
        Init();
    }

    FluidSimulator::~FluidSimulator()
    {

    }

    void FluidSimulator::TimeStep()
    {
        float dt = (float)Window::GetDeltaTime();
        AddForces(dt);
        Project(dt);
        AdvectVelocity(dt);
        AdvectSmoke(dt);
    }

    float* FluidSimulator::GetSmokeField()
    {
        return &_smokeField[0];
    }
}