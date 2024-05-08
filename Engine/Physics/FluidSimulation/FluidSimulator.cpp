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

    float FluidSimulator::BackwardEuler() const
    {
        //ToDo: Implement

        return 0;
    }

    float FluidSimulator::SemiLagrangian() const
    {
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
            PROFILE_SCOPE("AddForces");
            AddForces(dt);
        }
        {
            PROFILE_SCOPE("Project");
            Project(dt);
        }
        {
            PROFILE_SCOPE("AdvectVelocity");
            AdvectVelocity(dt);
        }
        {
            PROFILE_SCOPE("AdvectSmoke");
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
