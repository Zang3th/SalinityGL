#include "Timer.hpp"

namespace Engine
{
    // ----- Public -----

    Timer::Timer(double time_msec)
        :    _start(time_msec), _current(time_msec)
    {

    }

    void Timer::Update(const double dt_msec)
    {
        _current -= dt_msec;
    }

    bool Timer::CheckElapsedAndReset()
    {
        if(_current <= 0.0f)
        {
            _current = _start;
            return true;
        }

        return false;
    }
}