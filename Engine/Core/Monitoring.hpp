#pragma once

#include "Types.hpp"

#include <map>

namespace Engine
{
    class Monitoring
    {
        public:
            Monitoring() = delete;

            static void MinMaxAvgAt(const char* name, float value, uint32 x, uint32 y);
            static void Reset();

            inline static std::map<const char*, MinMaxAvg_t> values = std::map<const char*, MinMaxAvg_t>();
    };
}