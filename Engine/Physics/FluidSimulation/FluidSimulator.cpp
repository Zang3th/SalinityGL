#include "FluidSimulator.hpp"

namespace Engine
{
    // ----- Private -----

    float FluidSimulator::ForwardEuler(const float dt, const float u, const float q, const float q_right, const float q_left) const
    {
        /* The update step looks something like this:               *
         * q[i] = q[i] - dt * u[i] * (q[i+1] - q[i-1]) / (2 * dx);  *
         *   - q       := The quantity we want to advect.           *
         *   - q_right := q[i+1] - The right/upper neighbor.        *
         *   - q_left  := q[i-1] - The left/lower neighbor .        *
         *   - dt      := The timestep size.                        *
         *   - u       := The velocity (horizonal or vertical).     *
         *   - dx      := The cell size (1 in our simple case).     */

        return q - (dt * u * ((q_right - q_left) / (2 * _grid.dx)));
    }

    float FluidSimulator::BackwardEuler()
    {
        //ToDo: Implement

        return 0;
    }

    /* Applies forces to the liquid - just gravity in this case. */
    void FluidSimulator::AddForces(const float dt)
    {
        for(uint32 x = 0; x < _grid.width; x++)
        {
            for(uint32 y = 0; y < _grid.height; y++)
            {
                //Check for border cell
                if(_grid.s_At(x, y) != 0.0f && _grid.s_At(x, y-1) != 0.0f)
                {
                    _grid.v_At(x, y) += GRAVITY * dt;
                }
            }
        }
    }

    /* There shouldn't be a need for this function to exist, if Project() would work correctly. *
     * Furthermore, this function doesn't fix the problem.                                      */
    void FluidSimulator::CorrectForces()
    {
        for(uint32 x = 0; x < _grid.width; x++)
        {
            for(uint32 y = 0; y < _grid.height; y++)
            {
                if((x == 0) || (y == 0) || (x == _grid.width-1) || (y == _grid.height-1))
                {
                    _grid.u_At(x, y) = 0.0f;
                    _grid.v_At(x, y) = 0.0f;
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
                    if(_grid.s_At(x, y) == 0.0f)
                    {
                        continue;
                    }

                    //Get the amount of border cells in the area
                    const float rightNeighbor = _grid.s_At(x+1, y);
                    const float leftNeighbor  = _grid.s_At(x-1, y);
                    const float upperNeigbor  = _grid.s_At(x, y+1);
                    const float lowerNeighbor = _grid.s_At(x, y-1);

                    //Sum them up to divide the divergence by the correct amount
                    const float s_sum = rightNeighbor + leftNeighbor + upperNeigbor + lowerNeighbor;

                    //Skip if there are only solid cells
                    if(s_sum == 0.0f)
                    {
                        continue;
                    }

                    //Get the amount of fluid that is entering or leaving the cell
                    float divergence = _grid.u_At(x+1, y) - _grid.u_At(x, y) + _grid.v_At(x, y+1) - _grid.v_At(x, y);

                    //Calculate pressure
                    float pressure = divergence / s_sum;

                    //Apply overrelaxation to speed up convergence
                    pressure *= OVERRELAX;

                    //Push all velocities out by the same amout to force incompressibility
                    _grid.u_At(x, y)   += pressure * leftNeighbor;
                    _grid.u_At(x+1, y) -= pressure * rightNeighbor;
                    _grid.v_At(x, y)   += pressure * lowerNeighbor;
                    _grid.v_At(x, y+1) -= pressure * upperNeigbor;

                    //Monitor the pressure and the divergence to make sure that the fluid is incompressible
                    if(LiquiefiedParams::activateDebugging)
                    {
                        divergence = _grid.u_At(x+1, y) - _grid.u_At(x, y) + _grid.v_At(x, y+1) - _grid.v_At(x, y);

                        if(pressure < LiquefiedDebug::minPressure.val)
                            LiquefiedDebug::minPressure = {pressure, x, y};

                        if(pressure > LiquefiedDebug::maxPressure.val)
                            LiquefiedDebug::maxPressure = {pressure, x, y};

                        if(divergence < LiquefiedDebug::minDivergence.val)
                            LiquefiedDebug::minDivergence = {divergence, x, y};

                        if(divergence > LiquefiedDebug::maxDivergence.val)
                            LiquefiedDebug::maxDivergence = {divergence, x, y};

                    }
                }
            }
        }
    }

    /* Advection moves the quantity, or molecules, or particles, along the velocity field. */
    void FluidSimulator::AdvectVelocity(const float dt)
    {
        /* Iterate over grid and update all velocities according to the chosen numerical integrator. */
        for(uint32 x = 1; x < _grid.width-1; x++)
        {
            for(uint32 y = 1; y < _grid.height-1; y++)
            {
                //Skip border cells
                if(_grid.s_At(x, y) == 0)
                {
                    continue;
                }

                if(LiquiefiedParams::integratorChoice == Integrator::ForwardEuler)
                {
                    //ToDo: Look at the unstability of Forward-Euler.

                    const float newU = ForwardEuler(dt, _grid.u_Avg_At(x, y), _grid.u_At(x, y), _grid.u_At(x+1, y), _grid.u_At(x-1, y));
                    const float newV = ForwardEuler(dt, _grid.v_Avg_At(x, y), _grid.v_At(x, y), _grid.v_At(x, y+1), _grid.v_At(x, y-1));

                    _grid.u_temp_At(x, y) = newU; //u-component (horizontal advection)
                    _grid.v_temp_At(x, y) = newV; //v-component (vertical advection)
                }
                else if(LiquiefiedParams::integratorChoice == Integrator::BackwardEuler)
                {

                }
            }
        }

        _grid.UpdateGridVelocities();
    }

    /* To visualize the flow, we store a density value at the center of each cell  *
     * and advect it like the velocity field.                                      */
    void FluidSimulator::AdvectSmoke(const float dt)
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

                if(LiquiefiedParams::integratorChoice == Integrator::ForwardEuler)
                {
                    const float uResult = ForwardEuler(dt, _grid.u_Avg_At(x, y), _grid.smoke_At(x, y), _grid.smoke_At(x+1, y), _grid.smoke_At(x-1, y));
                    const float vResult = ForwardEuler(dt, _grid.v_Avg_At(x, y), _grid.smoke_At(x, y),_grid.smoke_At(x, y+1), _grid.smoke_At(x, y-1));

                    _grid.smoke_temp_At(x, y) = (uResult + vResult) / 2.0f;
                }
                else if(LiquiefiedParams::integratorChoice == Integrator::BackwardEuler)
                {

                }
            }
        }

        _grid.UpdateGridSmoke();
    }

    void FluidSimulator::Init()
    {
        for(uint32 x = 0; x < _grid.width; x++)
        {
            for(uint32 y = 0; y < _grid.height; y++)
            {
                _grid.u_At(x, y)          = 0.0f;
                _grid.u_temp_At(x, y)     = 0.0f;
                _grid.v_At(x, y)          = 0.0f;
                _grid.v_temp_At(x, y)     = 0.0f;
                _grid.s_At(x, y)          = 1.0f; //Fluid cells
                _grid.smoke_At(x, y)      = 1.0f; //Density value
                _grid.smoke_temp_At(x, y) = 0.0f;
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
        PROFILE_SCOPE("TimeStep");

        float dt = (float)Window::GetDeltaTime_sec();
        {
            PROFILE_SCOPE("AddForces");
            AddForces(dt);
        }
        {
            PROFILE_SCOPE("CorrectForces");
            //CorrectForces(); //Not necessary
        }
        {
            PROFILE_SCOPE("Diffuse");
            //Diffuse()        //?
        }
        {
            PROFILE_SCOPE("Project");
            Project(dt);
        }
        {
            PROFILE_SCOPE("AdvectVelocity");
            //AdvectVelocity(dt);
        }
        {
            PROFILE_SCOPE("AdvectSmoke");
            //AdvectSmoke(dt);
        }
        {
            PROFILE_SCOPE("Project");
            //Project(dt);
        }
    }

    void FluidSimulator::Reset()
    {
        Init();
        LiquefiedDebug::minPressure   = {FLT_MAX, 0, 0};
        LiquefiedDebug::maxPressure   = {FLT_MIN, 0, 0};
        LiquefiedDebug::minDivergence = {FLT_MAX, 0, 0};
        LiquefiedDebug::maxDivergence = {FLT_MIN, 0, 0};
    }

    void FluidSimulator::AddHorizonalTurbine(const uint32 x, const uint32 y, const float power)
    {
        if(x < _grid.width && y < _grid.height)
        {
            _grid.u_At(x, y) += power;
        }
    }

    StaggeredGrid* FluidSimulator::GetGrid()
    {
        return &_grid;
    }
}
