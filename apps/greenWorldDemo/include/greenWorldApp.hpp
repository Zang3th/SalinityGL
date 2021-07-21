#pragma once

#include "../../../engine-core/general/include/WindowManager.hpp"
#include "../../../engine-core/general/include/ResourceManager.hpp"
#include <memory>

class GreenWorldApp
{
    private:
        std::unique_ptr<WindowManager> _windowManager;

    public:
        GreenWorldApp();
        ~GreenWorldApp();
        bool IsRunning();
        void Update();
};