#pragma once

#include <chrono>
#include <map>

namespace Core
{
    class Profiler
    {
        private:
            const char* _scopeName;
            std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
            bool _stopped;

        public:
            Profiler(const char* name);
            ~Profiler();
            void Stop();
    };

    class ProfileResults
    {
        public:
            ProfileResults() = delete;
            inline static void AddFunctionScope(const char* func){_results[func] = 1.0f;}
            static std::map<const char*, float> _results;
    };

    #define PROFILE_SCOPE(name) Profiler profiler(name)
}