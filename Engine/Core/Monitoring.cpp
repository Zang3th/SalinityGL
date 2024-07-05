#include "Monitoring.hpp"

namespace Engine
{
    // ----- Public -----

    void Monitoring::LogToBuffer(const char* name, const float val, const uint32 x, const uint32 y)
    {
        //Add to normal monitoring
        buffer[_index] = {val, x, y};
        _index = (_index + 1) % LiquefiedParams::LIQUID_NUM_CELLS;

        Log(name, val, x, y);
    }

    void Monitoring::Log(const char* name, const float val, const uint32 x, const uint32 y)
    {
        std::string s_max = std::string(name).append("_max");
        std::string s_min = std::string(name).append("_min");

        //Check for new entry
        if(loggedValues.find(s_max) == loggedValues.end() &&
           loggedValues.find(s_min) == loggedValues.end())
        {
            loggedValues.emplace(std::pair<std::string, LogFloatXY_t>(s_max, {val, x, y}));
            loggedValues.emplace(std::pair<std::string, LogFloatXY_t>(s_min, {val, x, y}));
            return;
        }

        //Else check min and max
        if(val > loggedValues[s_max].val)
            loggedValues.at(s_max) = {val, x, y};

        if(val < loggedValues[s_min].val)
            loggedValues.at(s_min) = {val, x, y};
    }

    void Monitoring::Reset()
    {
        _index = 0;
        std::memset(&buffer[0], 0, LiquefiedParams::LIQUID_NUM_CELLS);
        _max = FLT_MIN;
        _min = FLT_MAX;
        loggedValues.clear();
    }
}
