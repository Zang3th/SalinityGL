#pragma once

#include <chrono>
#include <map>

namespace Core
{
    #define PROFILE_SCOPE(name) Profiler profiler(name)

    class Profiler
    {
        private:
            const char* _scopeName;
            std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
            bool _stopped;

        public:
            explicit Profiler(const char* name);
            ~Profiler();
            void Stop();
    };

    class ProfileResults
    {
        public:
            ProfileResults() = delete;
            inline static void AddFunctionScope(const char* func){_results[func] = 1.0f;}
            inline static std::map<const char*, float> _results = std::map<const char*, float>();
    };
}