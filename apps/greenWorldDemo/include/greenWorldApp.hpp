#pragma once

#include "../../../engine-core/general/include/WindowManager.hpp"

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