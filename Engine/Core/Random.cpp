#include "Random.hpp"

namespace Engine
{
    float Random::GetFloat()
    {
        return _distribution(_randomEngine);
    }
}