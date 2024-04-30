#include "Profiler.hpp"

namespace Engine
{
    // ----- Public -----

    Profiler::Profiler(const char* name)
        :   _scopeName(name)
    {
        //Check for new entry
        if(results.find(_scopeName) == results.end())
            results[_scopeName] = 0.0f;

        _startTime = std::chrono::high_resolution_clock::now();
    }

    Profiler::~Profiler()
    {
        const auto endTime = std::chrono::high_resolution_clock::now();

        const auto start = std::chrono::time_point_cast<std::chrono::microseconds>(_startTime).time_since_epoch().count();
        const auto end   = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

        const float duration = (float)(end - start) * 0.001f;
        results[_scopeName]  = duration;
    }
}