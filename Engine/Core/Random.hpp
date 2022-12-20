#pragma once

#include <random>
#include "Types.hpp"

namespace Engine
{
    class Random
    {
        private:
            static constexpr int FLOAT_MIN = 0;
            static constexpr int FLOAT_MAX = 1;

            inline static std::random_device         _randomDevice = std::random_device();
            inline static std::default_random_engine _randomEngine = std::default_random_engine(_randomDevice());
            inline static std::uniform_real_distribution<float> _distribution = std::uniform_real_distribution<float>(FLOAT_MIN, FLOAT_MAX);

        public:
            static float GetFloat();
    };
}