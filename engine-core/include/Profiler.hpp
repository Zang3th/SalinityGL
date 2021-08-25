#pragma once

#include <chrono>
#include <map>
#include <string>

namespace Core
{
    #define PROFILE_SCOPE(name) Profiler profiler(name)

    class Profiler
    {
        private:
            const std::string _scopeName;
            std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
            bool _stopped;

        public:
            explicit Profiler(const std::string& name);
            ~Profiler();
            void Stop();
    };

    class ProfileResults
    {
        public:
            ProfileResults() = delete;
            inline static void AddFunctionScope(const std::string& func){_results[func] = 1.0f;}
            inline static std::map<const std::string, float> _results = std::map<const std::string, float>();
    };
}