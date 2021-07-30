#pragma once

#include "../../../engine-core/general/include/WindowManager.hpp"
#include "../../../engine-core/general/include/ResourceManager.hpp"
#include "../../../engine-core/general/include/Renderer.hpp"
#include "../../../engine-core/general/include/CoreDefines.hpp"
#include "../../../engine-core/general/include/Sprite.hpp"
#include "../../../engine-core/general/include/Profiler.hpp"
#include "../../../engine-core/general/include/Camera.hpp"

#include "GW_Interface.hpp"
#include "GW_InputManager.hpp"

namespace GW
{
    #define INIT_GREENWORLD_APP INITIALIZE_EASYLOGGINGPP

    class App
    {
        private:
            Core::Ref<Core::WindowManager> _windowManager;
            Core::ResourceManager _resourceManager;
            Core::Ref<Core::Camera> _camera;
            Core::Scope<Interface> _userInterface;
            Core::Ref<Core::Renderer> _renderer;
            Core::Ref<Core::Sprite> _testSprite;

            void ConfigureLogger();
            void LoadResources();

        public:
            App();
            bool IsRunning();
            void Update();
    };
}