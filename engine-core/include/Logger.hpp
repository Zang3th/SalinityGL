#pragma once

#include "easylogging++.hpp"

namespace Core
{
    class Logger
    {
        public:
            Logger() = delete;
            static void Init();
    };
}