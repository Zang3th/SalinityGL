#include "FluidSimulator.hpp"

namespace Engine
{
    // ----- Private -----

    void FluidSimulator::Init() const
    {
        for(uint32 x = 0; x < _grid.width; x++)
        {
            for(uint32 y = 0; y < _grid.height; y++)
            {
                _grid.u    [AT(x, y)] = 0.0f;
                _grid.u_tmp[AT(x, y)] = 0.0f;
                _grid.v    [AT(x, y)] = 0.0f;
                _grid.v_tmp[AT(x, y)] = 0.0f;
                _grid.b    [AT(x, y)] = 1.0f; //1.0 for fluid cells and 0.0 for border cells
                _grid.d    [AT(x, y)] = 1.0f;
                _grid.d_tmp[AT(x, y)] = 0.0f;
            }
        }
    }

    /* Applies forces to the liquid - just gravity in this case. */
    void FluidSimulator::AddForces(const float dt) const
    {
        for(uint32 x = 0; x < _grid.width; x++)
        {
            for(uint32 y = 0; y < _grid.height; y++)
            {
                //Check for border cell
                if(_grid.b[AT(x, y)] != 0.0f && _grid.b[AT(x, y-1)] != 0.0f)
                {
                    _grid.v[AT(x, y)] += GRAVITY * dt;
                }
            }
        }
    }

    /* Projection calculates and applies the right amount of pressure to make the       *
     * velocity field divergence-free and also enforces solid wall boundary conditions. */
    void FluidSimulator::Project() const
    {
        //Solve iteratively using Gauss-Seidel
        for(uint32 it = 0; it < ITER; it++)
        {
            for(uint32 x = 1; x < _grid.width-1; x++)
            {
                for(uint32 y = 1; y < _grid.height-1; y++)
                {
                    //Skip border cells
                    if(_grid.b[AT(x, y)] == 0.0f)
                    {
                        continue;
                    }

                    //Get the amount of border cells in the area
                    const float rightNeighbor = _grid.b[AT(x+1, y)];
                    const float leftNeighbor  = _grid.b[AT(x-1, y)];
                    const float upperNeigbor  = _grid.b[AT(x, y+1)];
                    const float lowerNeighbor = _grid.b[AT(x, y-1)];

                    //Sum them up to divide the divergence by the correct amount
                    const float b_sum = rightNeighbor + leftNeighbor + upperNeigbor + lowerNeighbor;

                    //Skip if there are only border cells
                    if(b_sum == 0.0f)
                    {
                        continue;
                    }

                    //Get the amount of fluid that is entering or leaving the cell
                    float divergence = _grid.u[AT(x+1, y)] - _grid.u[AT(x, y)] + _grid.v[AT(x, y+1)] - _grid.v[AT(x, y)];

                    //Calculate pressure
                    float pressure = divergence / b_sum;

                    //Apply overrelaxation to speed up convergence
                    pressure *= OVERRELAX;

                    //Push all velocities out by the same amout to force incompressibility
                    _grid.u[AT(x, y)]   += pressure * leftNeighbor;
                    _grid.u[AT(x+1, y)] -= pressure * rightNeighbor;
                    _grid.v[AT(x, y)]   += pressure * lowerNeighbor;
                    _grid.v[AT(x, y+1)] -= pressure * upperNeigbor;

                    //Monitor the pressure and the divergence to make sure that the fluid is incompressible
                    if(LiquefiedParams::activateDebugging)
                    {
                        divergence = _grid.u[AT(x+1, y)] - _grid.u[AT(x, y)] + _grid.v[AT(x, y+1)] - _grid.v[AT(x, y)];

                        if(pressure < LiquefiedDebug::minPressure.val)
                        {
                            LiquefiedDebug::minPressure = {pressure, x, y};
                        }

                        if(pressure > LiquefiedDebug::maxPressure.val)
                        {
                            LiquefiedDebug::maxPressure = {pressure, x, y};
                        }

                        if(divergence < LiquefiedDebug::minDivergence.val)
                        {
                            LiquefiedDebug::minDivergence = {divergence, x, y};
                        }

                        if(divergence > LiquefiedDebug::maxDivergence.val)
                        {
                            LiquefiedDebug::maxDivergence = {divergence, x, y};
                        }
                    }
                }
            }
        }
    }

    /* Sets the values of the outmost cells to the values of neighbouring cells. */
    void FluidSimulator::Extrapolate() const
    {
        for(uint32 x = 0; x < _grid.width; x++)
        {
            _grid.u[AT(x, 0)] = _grid.u[AT(x, 1)];
            _grid.u[AT(x, _grid.height-1)] = _grid.u[AT(x, _grid.height-2)];
        }

        for(uint32 y = 0; y < _grid.height; y++)
        {
            _grid.v[AT(0, y)] = _grid.v[AT(1, y)];
            _grid.v[AT(_grid.width-1, y)] = _grid.v[AT(_grid.width-2, y)];
        }
    }

    /* Advection moves the quantity, or molecules, or particles, along the velocity field.      *
     * This function uses a semi-lagrangian approach while offering multiple numerical          *
     * integrators for calculating the previous x- and y-positions of a hypothetical particle   *
     * that is moving through the grid.                                                         */
    void FluidSimulator::AdvectVelocity(const float dt)
    {
        const float h  = _grid.h;
        const float h2 = _grid.h2;
        float uAdvect  = 0.0f;
        float vAdvect  = 0.0f;
        float ux_prev  = 0.0f;
        float uy_prev  = 0.0f;
        float vx_prev  = 0.0f;
        float vy_prev  = 0.0f;

        //Iterate over grid and update all velocities according to the chosen numerical integrator
        for(uint32 x = 1; x < _grid.width-1; x++)
        {
            for(uint32 y = 1; y < _grid.height-1; y++)
            {
                //Skip border cells
                if(_grid.b[AT(x, y)] == 0 || _grid.b[AT(x-1, y)] == 0 || _grid.b[AT(x, y-1)] == 0 || _grid.b[AT(x+1, y)] == 0 || _grid.b[AT(x, y+1)] == 0)
                {
                    continue;
                }

                // --- Calculate previous x- and y-positions depending on the chosen integrator

                if(LiquefiedParams::integratorChoice == Integrator::ForwardEuler)
                {
                    //Calculate u- and v-Advection for u-component
                    float u = _grid.u[AT(x, y)];
                    float v = _grid.v_Avg(x, y);

                    //Calculate approximated previous position for u-component
                    ForwardEuler(dt, (float)x, h, 0.0f, u, &ux_prev);
                    ForwardEuler(dt, (float)y, h, h2, v, &uy_prev);

                    //Calculate u- and v-Advection for v-component
                    u = _grid.u_Avg(x, y);
                    v = _grid.v[AT(x, y)];

                    //Calculate approximated previous position for v-component
                    ForwardEuler(dt, (float)x, h, h2, u, &vx_prev);
                    ForwardEuler(dt, (float)y, h, 0.0f, v, &vy_prev);
                }
                else if(LiquefiedParams::integratorChoice == Integrator::RungeKutta2)
                {
                    //Calculate u- and v-Advection for u-component
                    float u = _grid.u[AT(x, y)];
                    float v = _grid.v_Avg(x, y);

                    //Calculate approximated previous position for u-component
                    {
                        //Compute intermediate positions for k2 (corresponds to Forward-Euler with half the step size)
                        ForwardEuler(dt * 0.5f, (float)x, h, 0.0f, u, &ux_prev);
                        ForwardEuler(dt * 0.5f, (float)y, h, h2, v, &uy_prev);
                        ForwardEuler(dt * 0.5f, (float)x, h, h2, u, &vx_prev);
                        ForwardEuler(dt * 0.5f, (float)y, h, 0.0f, v, &vy_prev);

                        //Second stage: k2 (Sample grid at intermediate positions)
                        const float k2_u = _grid.SampleInterpolated(ux_prev, uy_prev, 0.0f, h2, _grid.u);
                        const float k2_v = _grid.SampleInterpolated(vx_prev, vy_prev, h2, 0.0f, _grid.v);

                        //Compute the final positions (corresponds to Forward-Euler with k2 as the input velocity)
                        ForwardEuler(dt, (float)x, h, 0.0f, k2_u, &ux_prev);
                        ForwardEuler(dt, (float)y, h, h2, k2_v, &uy_prev);
                    }

                    //Calculate u- and v-Advection for v-component
                    u = _grid.u_Avg(x, y);
                    v = _grid.v[AT(x, y)];

                    //Calculate approximated previous position for v-component
                    {
                        //Compute intermediate positions for k2 (corresponds to Forward-Euler with half the step size)
                        ForwardEuler(dt * 0.5f, (float)x, h, 0.0f, u, &ux_prev);
                        ForwardEuler(dt * 0.5f, (float)y, h, h2, v, &uy_prev);
                        ForwardEuler(dt * 0.5f, (float)x, h, h2, u, &vx_prev);
                        ForwardEuler(dt * 0.5f, (float)y, h, 0.0f, v, &vy_prev);

                        //Second stage: k2 (Sample grid at intermediate positions)
                        const float k2_u = _grid.SampleInterpolated(ux_prev, uy_prev, 0.0f, h2, _grid.u);
                        const float k2_v = _grid.SampleInterpolated(vx_prev, vy_prev, h2, 0.0f, _grid.v);

                        //Compute the final positions (corresponds to Forward-Euler with k2 as the input velocity)
                        ForwardEuler(dt, (float)x, h, h2, k2_u, &vx_prev);
                        ForwardEuler(dt, (float)y, h, 0.0f, k2_v, &vy_prev);
                    }
                }
                else if(LiquefiedParams::integratorChoice == Integrator::RungeKutta3)
                {
                    //...
                }

                //Sample grid on calculated previous positions
                uAdvect = _grid.SampleInterpolated(ux_prev, uy_prev, 0.0f, h2, _grid.u);
                vAdvect = _grid.SampleInterpolated(vx_prev, vy_prev, h2, 0.0f, _grid.v);

                //Assign new velocities
                _grid.u_tmp[AT(x, y)] = uAdvect; //u-component (horizontal advection)
                _grid.v_tmp[AT(x, y)] = vAdvect; //v-component (vertical advection)

                if(LiquefiedParams::activateDebugging)
                {
                    //Monitor stability
                    MonitorCFLStability(dt, uAdvect);
                    MonitorCFLStability(dt, vAdvect);

                    //Monitor velocities
                    if(uAdvect < LiquefiedDebug::minUAdvect.val)
                    {
                        LiquefiedDebug::minUAdvect = {uAdvect, x, y};
                    }

                    if(uAdvect > LiquefiedDebug::maxUAdvect.val)
                    {
                        LiquefiedDebug::maxUAdvect = {uAdvect, x, y};
                    }

                    if(vAdvect < LiquefiedDebug::minVAdvect.val)
                    {
                        LiquefiedDebug::minVAdvect = {vAdvect, x, y};
                    }

                    if(vAdvect > LiquefiedDebug::maxVAdvect.val)
                    {
                        LiquefiedDebug::maxVAdvect = {vAdvect, x, y};
                    }
                }
            }
        }

        //Swap buffers
        SWAP(_grid.u, _grid.u_tmp);
        SWAP(_grid.v, _grid.v_tmp);
    }

    /* To visualize the flow, we store a density value at the center of each cell  *
     * and advect it like the velocity field.                                      */
    void FluidSimulator::AdvectSmoke(const float dt)
    {
        const float h  = _grid.h;
        const float h2 = _grid.h2;
        float uAdvect  = 0.0f, vAdvect = 0.0f, x_prev = 0.0f, y_prev = 0.0f;

        for(uint32 x = 1; x < _grid.width-1; x++)
        {
            for(uint32 y = 1; y < _grid.height-1; y++)
            {
                //Skip border cells
                if(_grid.b[AT(x, y)] == 0)
                {
                    continue;
                }

                //Get u-and v-Advection
                uAdvect = (_grid.u[AT(x,y)] + _grid.u[AT(x+1,y)]) * 0.5f;
                vAdvect = (_grid.v[AT(x,y)] + _grid.v[AT(x,y+1)]) * 0.5f;

                // --- Calculate approximated previous x- and y-positions depending on the chosen integrator

                if(LiquefiedParams::integratorChoice == Integrator::ForwardEuler)
                {
                    ForwardEuler(dt, (float)x, h, h2, uAdvect, &x_prev);
                    ForwardEuler(dt, (float)y, h, h2, vAdvect, &y_prev);
                }
                else if(LiquefiedParams::integratorChoice == Integrator::RungeKutta2)
                {
                    //Compute intermediate positions for k2 (corresponds to Forward-Euler with half the step size)
                    ForwardEuler(dt * 0.5f, (float)x, h, h2, uAdvect, &x_prev);
                    ForwardEuler(dt * 0.5f, (float)y, h, h2, vAdvect, &y_prev);

                    //Second stage: k2 (Sample grid at intermediate positions)
                    const float k2_u = _grid.SampleInterpolated(x_prev, y_prev, 0.0f, h2, _grid.u);
                    const float k2_v = _grid.SampleInterpolated(x_prev, y_prev, h2, 0.0f, _grid.v);

                    //Compute the final positions (corresponds to Forward-Euler with k2 as the input velocity)
                    ForwardEuler(dt, (float)x, h, h2, k2_u, &x_prev);
                    ForwardEuler(dt, (float)y, h, h2, k2_v, &y_prev);
                }
                else if(LiquefiedParams::integratorChoice == Integrator::RungeKutta3)
                {
                    //...
                }

                //Assign new density
                _grid.d_tmp[AT(x, y)] = _grid.SampleInterpolated(x_prev, y_prev, h2, h2, _grid.d);
            }
        }

        //Swap buffer
        SWAP(_grid.d, _grid.d_tmp);
    }

    /* Monitors if the numerical integrator got unstable (value >= 1). */
    void FluidSimulator::MonitorCFLStability(const float dt, const float value) const
    {
        //Calculate numerical stability via CFL condition (Courant–Friedrichs–Lewy condition)
        const float cfl = (value * dt) / (float)_grid.h;

        //Save value for debugging purposes
        if(cfl > LiquefiedDebug::cflCondition)
        {
            LiquefiedDebug::cflCondition = cfl;
        }
    }

    void FluidSimulator::ForwardEuler(const float dt,  const float pos,
                                      const float h,   const float h2,
                                      const float vel, float* prev_pos)
    {
        /*      The update rule for Forward-Euler looks like this:                      *
        *                                                                               *
        *                        x1 = x0 - dt * f(x0)                                   *
        *                                                                               *
        *      - x1    := Approximated value at the next timestep.                      *
        *      - x0    := Value at the current timestep.                                *
        *      - dt    := The timestep size.                                            *
        *      - f(x0) := Derivative of f at the point x0 with respect to time,         *
        *                 which in this case is the velocity.                           *
        *                                                                               *
        *      Since we're working on a grid, we need to discretize it:                 *
        *                                                                               *
        *      - h     := The size of a grid cell in the corresponding dimension.       *
        *      - h2    := Half the size of a grid cell (used to find the midpoint).     *
        *                                                                               *
        *      The use of h and h2 helps adjust for the cell's center or edges.         *
        *      h2 is sometimes set to 0 when we are calculating the previous            *
        *      position at the edge of a cell, rather than the center.                  *
        *                                                                               *
        *      These are special cases because we are working on a staggered grid.      */

        *prev_pos = pos * h + h2 - dt * vel;
    }

    // void FluidSimulator::RungeKutta2( ... )
    // {
        /*      We wrote the update rule for Forward-Euler above like this:             *
        *                                                                               *
        *                        x1 = x0 - dt * f(x0)                                   *
        *                                                                               *
        *       Another way of writing it is given by the formula:                      *
        *                                                                               *
        *                       y_n+1 = y_n - h * f(t_n, y_n)                           *
        *                                                                               *
        *       - y_n+1       := Approximated value at the next step.                   *
        *       - y_n         := Value at the current step.                             *
        *       - h           := The step size.                                         *
        *       - f(t_n, y_n) := y'(t) := Derivative of y as a function of t and y.     *
        *                                                                               *
        *       Runge-Kutta 2 (also known as modified Forward-Euler or midpoint         *
        *       method) is an explicit second-order method with two stages.             *
        *       It is given by the formula:                                             *
        *                                                                               *
        *                       k1    = f(t_n, y_n)                                     *
        *                       k2    = f(t_n - h/2, y_n - h/2 * k1)                    *
        *                       y_n+1 = y_n - h * k2                                    */

        // Currently implemented using multiple steps of forward euler
    // }

    // ----- Public -----

    FluidSimulator::FluidSimulator()
    {
        Init();
    }

    void FluidSimulator::TimeStep(const float dt)
    {
        PROFILE_SCOPE("TimeStep");

        {
            PROFILE_SCOPE("#AddForces");
            AddForces(dt);
        }
        {
            PROFILE_SCOPE("#Project");
            Project();
        }
        {
            PROFILE_SCOPE("#Extrapolate");
            Extrapolate();
        }
        {
            PROFILE_SCOPE("#AdvectVelocity");
            AdvectVelocity(dt);
        }
        {
            PROFILE_SCOPE("#AdvectSmoke");
            AdvectSmoke(dt);
        }
    }

    void FluidSimulator::Reset() const
    {
        Init();
        LiquefiedDebug::Reset();
    }

    void FluidSimulator::AddBorderCell(uint32 x, uint32 y) const
    {
        _grid.b[AT(x, y)] = 0.0f;
    }

    void FluidSimulator::AddHorizonalTurbine(const uint32 x, const uint32 y, const float power, const float dt) const
    {
        _grid.u[AT(x, y)] += power * dt;
    }

    float FluidSimulator::GetDensity(uint32 x, uint32 y) const
    {
        return _grid.d[AT(x, y)];
    }

    float FluidSimulator::GetBorder(uint32 x, uint32 y) const
    {
        return _grid.b[AT(x, y)];
    }
}
