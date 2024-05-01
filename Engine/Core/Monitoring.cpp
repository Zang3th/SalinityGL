#include "Monitoring.hpp"

namespace Engine
{
    // ----- Public -----

    void Monitoring::MinMaxAvgAt(const char* name, const float value, const uint32 x, const uint32 y)
    {
        //Add new value if it's not already getting monitored
        if(values.find(name) == values.end())
        {
            values[name] = {value, value, value, x, y};
            return;
        }

        //Get current struct
        MinMaxAvg_t valueStruct = values[name];

        //Add min if it's less than current min
        if(valueStruct.min > value)
            valueStruct.min = value;
        //Add max if it's more than current max
        else if(valueStruct.max < value)
            valueStruct.max = value;

        //Add value and position
        valueStruct.val = value;
        valueStruct.x   = x;
        valueStruct.y   = y;

        //Assign new values
        values[name] = valueStruct;
    }

    void Monitoring::Reset()
    {
        for(auto& val : values)
            val.second = {0.0f, 0.0f, 0.0f, UINT32_MAX, UINT32_MAX};
    }
}