#include "FluidSimulator.hpp"

namespace Engine
{
    // ----- Private -----

    /* Applies forces to the liquid - just gravity in our case */
    void FluidSimulator::AddForces(float dt)
    {

    }

    /* Projection calculates and applies the right amount of pressure to make the       *
     * velocity field divergence-free and also enforces solid wall boundary conditions. */
    void FluidSimulator::Project(float dt)
    {

    }

    /* Advection moves the quantity, or molecules, or particles, along the velocity field. */
    void FluidSimulator::AdvectVelocity(float dt)
    {

    }

    /* To visualize the flow, we store a density or dye value at the center of each cell  *
     * and advect it like the velocity field.                                             */
    void FluidSimulator::AdvectSmoke(float dt)
    {
        
    }

    // ----- Public -----

    FluidSimulator::FluidSimulator()
    {

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