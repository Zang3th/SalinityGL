#pragma once

#include "../../../engine-core/general/include/WindowManager.hpp"
#include "../../../engine-core/general/include/ResourceManager.hpp"

class GreenWorldApp
{
    private:
        WindowManager* _windowManager;

    public:
        GreenWorldApp();
        ~GreenWorldApp();
        bool IsRunning();
        void Update();
};