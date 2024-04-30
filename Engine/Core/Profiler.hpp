#pragma once

#include <chrono>
#include <map>

namespace Engine
{
    #define PROFILE_SCOPE(name) Profiler profiler(name)

    class Profiler
    {
        private:
            const char* _scopeName;
            std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;

        public:
            explicit Profiler(const char* name);
            ~Profiler();

            inline static std::map<const char*, float> results = std::map<const char*, float>();
    };
}