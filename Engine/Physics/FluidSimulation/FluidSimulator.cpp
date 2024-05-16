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

    void FluidSimulator::Extrapolate()
    {
        //Setze die Werte der äußersten Zellen auf die ihrer Nachbarn

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
                if(_grid.b[AT(x, y)] == 0 || _grid.b[AT(x-1, y)] == 0 || _grid.b[AT(x, y-1)] == 0 || _grid.b[AT(x+1, y)] == 0 || _grid.b[AT(x, y+1)] == 0)
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
                    uAdvect = ModifiedForwardEuler(dt, _grid.u_Avg(x, y), _grid.u, x, y);
                    vAdvect = ModifiedForwardEuler(dt, _grid.v_Avg(x, y), _grid.v, x, y);

                    _grid.u_tmp[AT(x, y)] = uAdvect; //u-component (horizontal advection)
                    _grid.v_tmp[AT(x, y)] = vAdvect; //v-component (vertical advection)
                }
                else if(LiquiefiedParams::integratorChoice == Integrator::SemiLagrangian)
                {
                    // --- Set constants

                    /*int32 numX = 150;  // LiquiefiedParams::SIMULATION_WIDTH;
                    int32 numY = 100;  // LiquiefiedParams::SIMULATION_HEIGHT;



                    float dx = h2;
                    float dy = h2;

                    // --- Calculate new x and y based on velocities

                    uAdvect = _grid.u[AT(x,y)] + _grid.u[AT(x+1,y)] * 0.5f;
                    vAdvect = _grid.v[AT(x,y)] + _grid.v[AT(x,y+1)] * 0.5f;
                    float idx = (float)x * h + h2 - dt * uAdvect;
                    float idy = (float)y * h + h2 - dt * vAdvect;

                    float x_new = std::max(std::min(idx, ((float)numX * h)), h);
                    float y_new = std::max(std::min(idy, ((float)numY * h)), h);

                    _grid.d_tmp[AT(x, y)] = SemiLagrangian(x_new, y_new, dx, dy, _grid.d);*/

                    float h = 0.01f;   // (uint32)(1.0f / LiquiefiedParams::SIMULATION_HEIGHT);
                    float h2 = 0.005f; // 0.5f * h;

                    float u = _grid.u[AT(x, y)];
                    float v = _grid.v_Avg(x, y);
                    float ux = (float)x * h - dt * u;
                    float uy = (float)y * h + h2 - dt * v;

                    u = _grid.u_Avg(x, y);
                    v = _grid.v[AT(x, y)];
                    float vx = (float)x * h + h2 - dt * u;
                    float vy = (float)y * h - dt * v;

                    uAdvect = SemiLagrangian(ux, uy, 0.0f, h2, _grid.u);
                    vAdvect = SemiLagrangian(vx, vy, h2, 0.0f, _grid.v);

                    _grid.u_tmp[AT(x, y)] = uAdvect; //u-component (horizontal advection)
                    _grid.v_tmp[AT(x, y)] = vAdvect; //v-component (vertical advection)
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
        float uAdvect = 0.0f, vAdvect = 0.0f;

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
                    uAdvect = ForwardEuler(dt, _grid.u_Avg(x, y), _grid.d[AT(x, y)], _grid.d[AT(x+1, y)], _grid.d[AT(x-1, y)]);
                    vAdvect = ForwardEuler(dt, _grid.v_Avg(x, y), _grid.d[AT(x, y)], _grid.d[AT(x, y+1)], _grid.d[AT(x, y-1)]);

                    _grid.d_tmp[AT(x, y)] = (uAdvect + vAdvect) / 2.0f;
                }
                else if(LiquiefiedParams::integratorChoice == Integrator::BackwardEuler)
                {
                    uAdvect = ModifiedForwardEuler(dt, _grid.u_Avg(x, y), _grid.d, x, y);
                    vAdvect = ModifiedForwardEuler(dt, _grid.v_Avg(x, y), _grid.d, x, y);

                    _grid.d_tmp[AT(x, y)] = (uAdvect + vAdvect) / 2.0f;
                }
                else if(LiquiefiedParams::integratorChoice == Integrator::SemiLagrangian)
                {
                    //if (this.s[i*n + j] != 0.0) {
                    //    var u = (this.u[i*n + j] + this.u[(i+1)*n + j]) * 0.5;
                    //    var v = (this.v[i*n + j] + this.v[i*n + j+1]) * 0.5;
                    //    var x = i*h + h2 - dt*u;
                    //    var y = j*h + h2 - dt*v;

                    //    this.newM[i*n + j] = this.sampleField(x,y, S_FIELD);
                    //}

                    // --- Set constants

                    int32 numX = 150;  // LiquiefiedParams::SIMULATION_WIDTH;
                    int32 numY = 100;  // LiquiefiedParams::SIMULATION_HEIGHT;

                    float h = 0.01f;   // (uint32)(1.0f / LiquiefiedParams::SIMULATION_HEIGHT);
                    float h2 = 0.005f; // 0.5f * h;

                    float dx = h2;
                    float dy = h2;

                    // --- Calculate new x and y based on velocities

                    uAdvect = (_grid.u[AT(x,y)] + _grid.u[AT(x+1,y)]) * 0.5f;
                    vAdvect = (_grid.v[AT(x,y)] + _grid.v[AT(x,y+1)]) * 0.5f;
                    float ux = (float)x * h + h2 - dt * uAdvect;
                    float vy = (float)y * h + h2 - dt * vAdvect;

                    _grid.d_tmp[AT(x, y)] = SemiLagrangian(ux, vy, dx, dy, _grid.d);
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

        const uint32 dx = _grid.dx;
        float velocity = q - dt * u * ((q_next - q_prev) / (float)(2 * dx));

        if(LiquiefiedParams::activateDebugging)
        {
            //Check numerical stability via CFL condition (Courant–Friedrichs–Lewy condition)
            const float cfl = (velocity * dt) / (float)dx;

            //Save value for debugging purposes
            if(cfl > LiquefiedDebug::cflCondition)
                LiquefiedDebug::cflCondition = cfl;

            //Forward-Euler has gotten unstable - return 0
            if(LiquefiedDebug::cflCondition >= 1.0f)
                velocity = 0.0f;
        }

        return velocity;
    }

    float FluidSimulator::ModifiedForwardEuler(float dt, float vel, const float* q_values, uint32 q_startX, uint32 q_startY) const
    {
        const uint32 dx     = _grid.dx;
        const float  h_half = dt / 2;

        uint32 idx_left  = AT(q_startX + dx, q_startY);
        uint32 idx_right = AT(q_startX - dx, q_startY);
        float k1 = (q_values[idx_left] - q_values[idx_right]) / (float)(2 * dx);

        uint32 x_left  = q_startX + dx + (uint32)(h_half * k1);
        uint32 x_right = q_startX - dx - (uint32)(h_half * k1);
        idx_left  = AT(x_left, q_startY);
        idx_right = AT(x_right, q_startY);
        const float k2 = (q_values[idx_left] - q_values[idx_right]) / (float)(2 * dx) + h_half;

        float result = q_values[AT(q_startX, q_startY)] - dt * vel * k2;

        if(LiquiefiedParams::activateDebugging)
        {
            //Check numerical stability via CFL condition (Courant–Friedrichs–Lewy condition)
            const float cfl = (result * dt) / (float)dx;

            //Save value for debugging purposes
            if(cfl > LiquefiedDebug::cflCondition)
                LiquefiedDebug::cflCondition = cfl;

            //Forward-Euler has gotten unstable - return 0
            if(LiquefiedDebug::cflCondition >= 1.0f)
                result = 0.0f;
        }

        return result;
    }

    float FluidSimulator::BackwardEuler(const float dt, const float vel, const float* q_values, const uint32 q_startX, const uint32 q_startY) const
    {
       /*       The update rule for Backward-Euler looks like this:                 *
        *                                                                           *
        *       (1.)                 x1 = x0 + dt * f(x1)                           *
        *                                                                           *
        *       - x1    := Approximated value at the next timestep.                 *
        *       - x0    := Value at the current timestep.                           *
        *       - dt    := The timestep size.                                       *
        *       - f(x1) := Derivative of f at the point x1 with respect to time.    *
        *                                                                           *
        *       f(x1) makes this method implicit because we do not know x1 at the   *
        *       current moment in time, we were trying to solve for x1!             *
        *       So how do we go on about this?!                                     *
        *                                                                           *
        *       To solve for x1, we need to employ numerical methods.               *
        *       For linear problems, direct algebraic manipulation may suffice.     *
        *       For nonlinear problems, iterative methods are commonly used.        *
        *                                                                           *
        *       Let's start by discretizing Backward-Euler:                         *
        *                                                                           *
        *       (2.)                q1[i] = q0[i] + dt * f(q1[i])                   *
        *                                                                           *
        *       We can formulate Backward-Euler as a root finding problem:          *
        *                                                                           *
        *       (3.)            F = q1[i] - q0[i] - dt * f(q1[i]) = 0               *
        *                                                                           *
        *       For an equation like this we can use the Newton-Raphson method.     *
        *       The update rule for Newton-Raphson looks like this:                 *
        *                                                                           *
        *       (4.)            x_new = x_old - f(x_old) / f'(x_old)                *
        *                                                                           *
        *       - x_new     := Approximated value for the next iteration.           *
        *       - x_old     := Value at the current iteration.                      *
        *       - f(x_old)  := Value of f at the point x_old.                       *
        *       - f'(x_old) := Derivative of f at the point x_old.                  *
        *                                                                           *
        *       This involves guessing an initial value for x_new (like x0),        *
        *       computing the derivative of f with respect to x_old, and            *
        *       iteratively refining x_new until the change becomes very small.     *
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
        *       Using a nonlinear solver for each step of Backward-Euler:           *
        *       Source: Cornell University - CS3220 Lecture Notes                   *
        *       https://www.cs.cornell.edu/~bindel/class/cs3220-s12/lectures.html   */

        const uint32 dx  = _grid.dx;
        const uint32 x1  = NewtonRaphson(q_values, q_startX, q_startY, 5);
        const float f_x1 = (q_values[x1+dx] - q_values[x1-dx]) / (float)(2 * dx);
        const float x0   = q_values[AT(q_startX, q_startY)];

        float velocity = x0 - dt * vel * f_x1;

        if(LiquiefiedParams::activateDebugging)
        {
            //Check numerical stability via CFL condition (Courant–Friedrichs–Lewy condition)
            const float cfl = (velocity * dt) / (float)dx;

            //Save value for debugging purposes
            if(cfl > LiquefiedDebug::cflCondition)
                LiquefiedDebug::cflCondition = cfl;

            //Backward-Euler has gotten unstable - return 0
            if(LiquefiedDebug::cflCondition >= 1.0f)
                velocity = 0.0f;
        }

        return velocity;
    }

    uint32 FluidSimulator::NewtonRaphson(const float* values, const uint32 startX, const uint32 startY, const uint32 maxIteration) const
    {
        /*      The update rule for Newton-Raphson looks like this:                 *
         *                                                                          *
         *                          x1 = x0 - f(x0) / f'(x0)                        *
         *                                                                          *
         *      - x1     := Approximated x-value for the next iteration.            *
         *      - x0     := x-value at the current iteration.                       *
         *      - f(x0)  := Value of f at the point x0.                             *
         *      - f'(x0) := Derivative of f at the point x0.                        *
         *                                                                          *
         *      We will use a central derivative approximation just like before.    */

        const uint32 dx = _grid.dx;
        float f_x0 = 0.0f, f_x1 = 0.0f;
        uint32 x1 = 0, x0 = AT(startX, startY);

        for(uint32 iter = 0; iter < maxIteration; iter++)
        {
            f_x0 = values[x0];
            f_x1 = (values[x0+dx] - values[x0-dx]) / (float)(2 * dx);
            x1   = x0 - (uint32)(f_x0 / f_x1);

            //Check convergence, break if value didn't change
            if(x1 == x0)
                break;

            x0 = x1;
        }

        return x1;
    }

    float FluidSimulator::SemiLagrangian(const float x, const float y, const float dx, const float dy, const float* f) const
    {
        // --- Set constants

        int32 numX = 150;  // LiquiefiedParams::SIMULATION_WIDTH;
        int32 numY = 100;  // LiquiefiedParams::SIMULATION_HEIGHT;

        float h = 0.01f;   // (uint32)(1.0f / LiquiefiedParams::SIMULATION_HEIGHT);

        // --- Calculate new x and new y

        float x_new = std::max(std::min(x, ((float)numX * h)), h);
        float y_new = std::max(std::min(y, ((float)numY * h)), h);

        // --- Sample field

        int32 x0 = (int32)std::min(std::floor((x_new-dx)*(float)numY), (float)numX-1);
        x0 = std::max(x0, 0);
        int32 x1 = std::min(x0+1, numX-1);
        float tx = ((x_new-dx) - (float)x0*h) * (float)numY;

        int32 y0 = (int32)std::min(std::floor((y_new-dy)*(float)numY), (float)numY-1);
        y0 = std::max(y0, 0);
        int32 y1 = std::min(y0+1, numY-1);
        float ty = ((y_new-dy) - (float)y0*h) * (float)numY;

        float sx = 1.0f - tx;
        float sy = 1.0f - ty;

        float val = sx*sy * f[AT(x0, y0)] +
                    tx*sy * f[AT(x1, y1)] +
                    tx*ty * f[AT(x1, y1)] +
                    sx*ty * f[AT(x0, y1)];

        return val;
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
            PROFILE_SCOPE("#Extrapolate");
            //Extrapolate();
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
