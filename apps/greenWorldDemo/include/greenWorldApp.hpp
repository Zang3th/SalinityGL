#pragma once

#include "../../../engine-core/general/include/WindowManager.hpp"
#include "../../../engine-core/general/include/ResourceManager.hpp"
#include "../../../engine-core/general/include/Renderer.hpp"
#include "../../../engine-core/general/include/CoreDefines.hpp"
#include "../../../engine-core/general/include/Sprite.hpp"
#include "greenWorldInterface.hpp"

class GreenWorldApp
{
    private:
        Core::Ref<Core::WindowManager> _windowManager;
        Core::ResourceManager _resourceManager;
        Core::Scope<GreenWorldInterface> _userInterface;
        Core::Renderer _renderer;
        Core::Ref<Core::Sprite> _testSprite;

        void LoadResources();

    public:
        GreenWorldApp();
        bool IsRunning();
        void Update();
};