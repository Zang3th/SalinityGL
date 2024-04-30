#include "Monitor.hpp"

namespace Engine
{
    // ----- Public -----

    void Monitor::MinMaxAvg(const char* name, const float value)
    {
        //Add new value if it's not already getting monitored
        if(values.find(name) == values.end())
        {
            values[name] = {value, value, value};
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

        //Add value
        valueStruct.val = value;

        //Assign new values
        values[name] = valueStruct;
    }

    void Monitor::Reset()
    {
        values.clear();
    }
}