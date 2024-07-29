#include "Benchmark.hpp"

namespace Engine
{
    // ----- Private -----

    void Benchmark::End()
    {
        //If benchmark file doesn't exist, create it

        //Append monitored values

        Logger::Info("Logged", "File", _filename);
    }

    // ----- Public -----

    void Benchmark::Start(const std::string& appName, const uint32 time_msec)
    {
        //Init variables
        _isRunning = true;
        _appName   = appName;
        _count     = 0;
        _avg       = 0.0;
        _min       = FLT_MAX;
        _max       = FLT_MIN;
        _timer     = MakeScope<Timer>(time_msec);

        Logger::Info("Started", "Benchmark", _appName);
    }

    void Benchmark::AddFrame(const double dt_msec)
    {
        _timer->Update(dt_msec);

        //Check if timer has elapsed, else monitor values
        if(_timer->CheckElapsedAndReset() == true)
        {
            //Calculate average, end benchmark and reset states
            _avg /= (double)_count;
            End();
            _isRunning = false;
            UIParams::runBenchmark = false;

            Logger::Info("Finished", "Benchmark", _appName);
        }
        else
        {
            _count++;
            _avg += dt_msec;

            if(_min > dt_msec)
                _min = dt_msec;

            if(_max < dt_msec)
                _max = dt_msec;
        }
    }

    bool Benchmark::IsRunning()
    {
        return _isRunning;
    }
}
