#include "Profiler.hpp"

namespace Core
{
    std::map<const char*, float> ProfileResults::_results;

    Profiler::Profiler(const char* name)
        : _scopeName(name), _stopped(false)
    {
        _startTime = std::chrono::high_resolution_clock::now();
    }

    Profiler::~Profiler()
    {
        if(!_stopped)
            Stop();
    }

    void Profiler::Stop()
    {
        auto endTime = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(_startTime).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

        _stopped = true;

        float duration = (float)(end - start) * 0.001f;
        ProfileResults::_results[_scopeName] = duration;
    }
}