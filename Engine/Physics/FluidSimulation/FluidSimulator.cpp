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
                _grid.d    [AT(x, y)] = 1.0f; //Density value
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
    void FluidSimulator::Project(const float dt) const
    {
        //Simple solution using Gauss-Seidel
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
                    if(LiquiefiedParams::activateDebugging)
                    {
                        divergence = _grid.u[AT(x+1, y)] - _grid.u[AT(x, y)] + _grid.v[AT(x, y+1)] - _grid.v[AT(x, y)];

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
        float uAdvect = 0.0f, vAdvect = 0.0f;

        /* Iterate over grid and update all velocities according to the chosen numerical integrator. */
        for(uint32 x = 1; x < _grid.width-1; x++)
        {
            for(uint32 y = 1; y < _grid.height-1; y++)
            {
                //Skip border cells
                if(_grid.b[AT(x, y)] == 0)
                {
                    continue;
                }

                if(LiquiefiedParams::integratorChoice == Integrator::ForwardEuler)
                {
                    uAdvect = ForwardEuler(dt, _grid.u_Avg(x, y), _grid.u[AT(x, y)], _grid.u[AT(x+1, y)], _grid.u[AT(x-1, y)]);
                    vAdvect = ForwardEuler(dt, _grid.v_Avg(x, y), _grid.v[AT(x, y)], _grid.v[AT(x, y+1)], _grid.v[AT(x, y-1)]);

                    _grid.u_tmp[AT(x, y)] = uAdvect; //u-component (horizontal advection)
                    _grid.v_tmp[AT(x, y)] = vAdvect; //v-component (vertical advection)
                }
                else if(LiquiefiedParams::integratorChoice == Integrator::BackwardEuler)
                {

                }

                //Monitor the applied horizontal and vertical advection
                if(LiquiefiedParams::activateDebugging)
                {
                    if(uAdvect < LiquefiedDebug::minUAdvect.val)
                        LiquefiedDebug::minUAdvect = {uAdvect, x, y};

                    if(uAdvect > LiquefiedDebug::maxUAdvect.val)
                        LiquefiedDebug::maxUAdvect = {uAdvect, x, y};

                    if(vAdvect < LiquefiedDebug::minVAdvect.val)
                        LiquefiedDebug::minVAdvect = {vAdvect, x, y};

                    if(vAdvect > LiquefiedDebug::maxVAdvect.val)
                        LiquefiedDebug::maxVAdvect = {vAdvect, x, y};
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
        for(uint32 x = 1; x < _grid.width-1; x++)
        {
            for(uint32 y = 1; y < _grid.height-1; y++)
            {
                //Skip border cells
                if(_grid.b[AT(x, y)] == 0)
                {
                    continue;
                }

                if(LiquiefiedParams::integratorChoice == Integrator::ForwardEuler)
                {
                    const float uResult = ForwardEuler(dt, _grid.u_Avg(x, y), _grid.d[AT(x, y)], _grid.d[AT(x+1, y)], _grid.d[AT(x-1, y)]);
                    const float vResult = ForwardEuler(dt, _grid.v_Avg(x, y), _grid.d[AT(x, y)], _grid.d[AT(x, y+1)], _grid.d[AT(x, y-1)]);

                    _grid.d_tmp[AT(x, y)] = (uResult + vResult) / 2.0f;
                }
                else if(LiquiefiedParams::integratorChoice == Integrator::BackwardEuler)
                {

                }
            }
        }

        //Swap buffer
        SWAP(_grid.d, _grid.d_tmp);
    }

    float FluidSimulator::ForwardEuler(const float dt, const float u, const float q, const float q_next, const float q_prev) const
    {
        /*      The update rule for Forward-Euler looks like this:                  *
         *                                                                          *
         *                        x1 = x0 + dt * f(x0)                              *
         *                                                                          *
         *      - x1    := Approximated value at the next timestep.                 *
         *      - x0    := Value at the current timestep.                           *
         *      - dt    := The timestep size.                                       *
         *      - f(x0) := Derivative of f at the point x0 with respect to time.    *
         *                                                                          *
         *      We will use a central spatial derivative approximation:             *
         *                                                                          *
         *              f(x0) = (f(x0+h) - f(x0-h)) / (2 * h)                       *
         *                                                                          *
         *                      Our formula then becomes:                           *
         *                                                                          *
         *               x1 = x0 + dt * f(x0+h) - f(x0-h) / 2 * h                   *
         *                                                                          *
         *      Since we're working on a grid, we need to discretize it:            *
         *                                                                          *
         *      q1[i] = q0[i] + dt * (q0[i+1] - q0[i-1]) / (2 * dx)                 *
         *                                                                          *
         *      - q      := The quantity we want to advect.                         *
         *      - q1[i]  := Value at the next timestep.                             *
         *      - q0[i]  := Value at the current timestep.                          *
         *      - q_next := q0[i+1] := The right or upper neighbor.                 *
         *      - q_left := q0[i-1] := The left or lower neighbor.                  *
         *      - dx     := The cell size.                                          *
         *                                                                          *
         *      Because we use Forward-Euler for Advection (Movement along the      *
         *      velocity field) we need to multiply with u (the velocity).          *
         *      Furthermore subtracting from q0 instead of adding yields better     *
         *      stability in this case. Our final formula then becomes:             *
         *                                                                          *
         *      q1[i] = q0[i] - dt * u[i] * (q0[i+1] - q0[i-1]) / (2 * dx)          */

        const float dx = _grid.dx;
        float velocity = q - dt * u * ((q_next - q_prev) / (2 * dx));

        if(LiquiefiedParams::activateDebugging)
        {
            //Check numerical stability via CFL condition (Courant–Friedrichs–Lewy condition)
            const float cfl = (velocity * dt) / dx;

            //Forward-Euler has gotten unstable - return 0
            if(LiquefiedDebug::cflCondition >= 1.0f)
                velocity = 0.0f;

            //Save value for debugging purposes
            if(cfl > LiquefiedDebug::cflCondition)
                LiquefiedDebug::cflCondition = cfl;
        }

        return velocity;
    }

    float FluidSimulator::BackwardEuler() const
    {
       /*       The update rule for Backward-Euler looks like this:                 *
        *                                                                           *
        *                        x1 = x0 + dt * f(x1)                               *
        *       - x1    := Approximated value at the next timestep.                 *
        *       - x0    := Value at the current timestep.                           *
        *       - dt    := The timestep size.                                       *
        *       - f(x1) := Derivative of f at the point x1 with respect to time.    *
        *                                                                           *
        *       f(x1) makes this method implicit because we dont know x1 at the     *
        *       current moment in time, we were trying to solve for x1!             *
        *       So how do we go on about this?!                                     *
        *                                                                           *
        *       To solve for x1, we need to employ numerical methods.               *
        *       For linear problems, direct algebraic manipulation may suffice.     *
        *       For nonlinear problems, iterative methods are commonly used.        *
        *                                                                           *
        *       We will use the Newton-Raphson method.                              *
        *       This involves guessing an initial value for x1, computing the       *
        *       derivative of f with respect to x1, and iteratively refining the    *
        *       estimate of x1 until the change becomes negligibly small.           *
        *                                                                           *
        *                               Algorithmic steps:                          *
        *                                                                           *
        *       1. Calculate a satisfying approximation for f(x1) with              *
        *          the Newton-Raphson method.                                       *
        *       2. Insert f(x1) back into the Backward-Euler update rule to         *
        *          calculate the value for the next timestep.                       *
        *                                                                           *
        *       With the cost of this additional computational effort comes         *
        *       the advantage of way higher stability and robustness than with      *
        *       Forward-Euler.                                                      *
        *                                                                           *
        *                               Notes:                                      *
        *                                                                           *
        *       - We will use a central derivative approximation just like before.  *
        *       - We will discretize the equations according to our grid just       *
        *         like before.                                                      */

        //ToDo: Implement

        return 0;
    }

    float FluidSimulator::SemiLagrangian() const
    {
        //ToDo: Implement

        return 0;
    }


    float FluidSimulator::NewtonRaphson() const
    {
        /*                               Algorithmic steps:                         *
         *                                                                          *
         *      1. Get an initial guess for x1 => like x1 = x0.                     *
         *      2. ToDo: ...                                                        *
         *                                                                          */

        //ToDo: Implement

        return 0;
    }

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
            Project(dt);
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
}
