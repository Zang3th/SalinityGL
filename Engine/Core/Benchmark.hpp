#pragma once

#include "Timer.hpp"
#include "Logger.hpp"
#include "GlobalParams.hpp"

#include <string>

namespace Engine
{
    class Benchmark
    {
        private:
            inline static constexpr const char* _filename = "Benchmark.md";

            inline static bool         _isRunning = bool();
            inline static std::string  _appName   = std::string();
            inline static uint64       _count     = uint64();
            inline static double       _avg = double(), _min = double(), _max = double();
            inline static Scope<Timer> _timer;

            static void End();

        public:
            Benchmark() = delete;
            static void Start(const std::string& appName, uint32 time_msec);
            static void AddFrame(double dt_msec);

            [[nodiscard]] static bool IsRunning();
    };
}
