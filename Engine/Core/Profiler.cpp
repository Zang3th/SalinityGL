#include "Profiler.hpp"

namespace Engine
{
    // ----- Public -----

    Profiler::Profiler(const char* name)
        :   _scopeName(name)
    {
        //Check for new entry
        if(_results.find(_scopeName) == _results.end())
            _results[_scopeName] = 0.0f;

        _startTime = std::chrono::high_resolution_clock::now();
    }

    Profiler::~Profiler()
    {
        auto endTime = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(_startTime).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

        float duration = (float)(end - start) * 0.001f;
        _results[_scopeName] = duration;
    }
}