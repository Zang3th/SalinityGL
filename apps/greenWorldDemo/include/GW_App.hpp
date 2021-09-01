#pragma once

#include "Logger.hpp"
#include "Window.hpp"
#include "ResourceManager.hpp"
#include "Renderer.hpp"
#include "CoreDefines.hpp"
#include "Sprite.hpp"
#include "Profiler.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "MeshCreator.hpp"
#include "Model.hpp"
#include "Cubemap.hpp"
#include "Heightmap.hpp"
#include "Audio.hpp"
#include "Sprite.hpp"
#include "ShadowRenderer.hpp"

#include "GW_Interface.hpp"
#include "GW_InputManager.hpp"

#include <vector>
#include <array>

namespace GW
{
    class App
    {
        private:
            Core::Scope<Core::Window>           _window;
            Core::Scope<Core::Renderer>         _renderer;
            Core::Scope<Core::Camera>           _camera;
            Core::Scope<Core::Audio>            _audio;
            Core::Scope<Core::ShadowRenderer>   _shadowRenderer;
            Core::Scope<Core::Cubemap>          _cubemap;
            Core::Scope<Core::Sprite>           _testSprite;
            std::vector<Core::Model>            _models;

            Core::Scope<Interface>              _interface;

            void InitModules();
            void CreateModels();
            void CreateCubemap();
            void CreateSprites();

        public:
            App();
            ~App();
            bool IsRunning();
            void Update();
    };
}