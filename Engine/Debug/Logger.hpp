#pragma once

#include "easylogging++.hpp"

namespace Engine
{
    class Logger
    {
        public:
            Logger() = delete;
            static void Init();
    };
}