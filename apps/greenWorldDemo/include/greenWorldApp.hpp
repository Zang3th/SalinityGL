#pragma once

#include "../../../engine-core/general/include/WindowManager.hpp"
#include "../../../engine-core/general/include/ResourceManager.hpp"
#include "../../../engine-core/general/include/CoreDefines.hpp"
#include "../../../engine-core/general/include/Sprite.hpp"

class GreenWorldApp
{
    private:
        Core::WindowManager _windowManager;
        Core::ResourceManager _resourceManager;
        Core::Scope<Core::Sprite> _testSprite;

        void LoadResources();

    public:
        GreenWorldApp();
        bool IsRunning();
        void Update();
};