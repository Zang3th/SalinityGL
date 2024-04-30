#pragma once

#include "Types.hpp"

#include <map>

namespace Engine
{
    class Monitor
    {
        public:
            Monitor() = delete;

            static void MinMaxAvg(const char* name, float value);
            static void Reset();

            inline static std::map<const char*, MinMaxAvg_t> values = std::map<const char*, MinMaxAvg_t>();
    };
}