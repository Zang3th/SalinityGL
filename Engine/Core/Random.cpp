#include "Random.hpp"

namespace Engine
{
    float Random::GetFloat()
    {
        return _distribution(_randomEngine);
    }

    int32 Random::GetInt32_Pseudo(int32 upperLimit)
    {
        return std::rand() % upperLimit;
    }
}