#include "FluidSimulator.hpp"

namespace Engine
{
    // ----- Private -----

    float FluidSimulator::ForwardEuler(const float dt, const float dx, const float vel_field, const float q, const float q_right, const float q_left)
    {
        return q - (dt * vel_field * ((q_right - q_left) / (2 * dx)));
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
        /* Iterate over grid and update all velocities according to our numerical integrator.                 *
         * For now we use Forward-Euler (unstable!) for the time derivative and a central spatial derivative. *
         *                                                                                                    *
         * Our Update-Algorithm will look something like this:                                                *
         * q[i] = q[i] - delta_t * u[i] * (q[i+1] - q[i-1]) / (2 * delta_x);                                  *
         *   - q := The quantity we want to advect.                                                           *
         *   - delta_t := The timestep size.                                                                  *
         *   - u := The horizontal velocity (we need to advect the vertical velocity v as well).              *
         *   - delta_x := The cell size (1 in our simple case).                                               */

        for(uint32 x = 1; x < _grid.width-1; x++)
        {
            for(uint32 y = 1; y < _grid.height-1; y++)
            {
                //Skip border cells
                if(_grid.s_At(x, y) == 0)
                {
                    continue;
                }

                //u-component (horizontal advection)
                _grid.u_temp_At(x, y) = ForwardEuler(dt, _grid.deltaX, _grid.u_At(x, y), _grid.u_At(x, y), _grid.u_At(x+1, y), _grid.u_At(x-1, y));

                //v-component (vertical advection)
                _grid.v_temp_At(x, y) = ForwardEuler(dt, _grid.deltaY, _grid.v_At(x, y), _grid.v_At(x, y), _grid.v_At(x, y+1), _grid.v_At(x, y-1));
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

                //u-component (horizontal advection)
                const float u_result = ForwardEuler(dt, _grid.deltaX, _grid.u_At(x, y), _grid.smoke_At(x, y), _grid.smoke_At(x+1, y), _grid.smoke_At(x-1, y));

                //v-component (vertical advection)
                const float v_result = ForwardEuler(dt, _grid.deltaY, _grid.v_At(x, y), _grid.smoke_At(x, y),_grid.smoke_At(x, y+1), _grid.smoke_At(x, y-1));

                _grid.smoke_temp_At(x, y) = (u_result + v_result) / 2.0f;
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
        float dt = (float)Window::GetDeltaTime();
        AddForces(dt);
        Project(dt);
        AdvectVelocity(dt);
        AdvectSmoke(dt);
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