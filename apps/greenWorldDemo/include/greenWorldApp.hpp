#pragma once

#include "../../../engine-core/general/include/WindowManager.hpp"
#include "../../../engine-core/general/include/ResourceManager.hpp"

class GreenWorldApp
{
    private:
        Core::WindowManager _windowManager;
        Core::ResourceManager _resourceManager;

        void LoadResources();

    public:
        GreenWorldApp();
        bool IsRunning();
        void Update();
};