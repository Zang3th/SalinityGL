#pragma once

#include "Types.hpp"
#include "GlobalParams.hpp"

#include <cstring>
#include <map>
#include <string>

namespace Engine
{
    class Monitoring
    {
        private:
            inline static uint32 _index = 0;
            inline static float  _max   = FLT_MIN;
            inline static float  _min   = FLT_MAX;

        public:
            inline static LogFloatXY buffer[LiquefiedParams::LIQUID_NUM_CELLS] = {};
            inline static std::map<std::string, LogFloatXY_t> loggedValues = std::map<std::string, LogFloatXY_t>();

            Monitoring() = delete;

            static void LogToBuffer(const char* name, float val, uint32 x, uint32 y);
            static void Log(const char* name, float val, uint32 x, uint32 y);
            static void Reset();
    };
}
