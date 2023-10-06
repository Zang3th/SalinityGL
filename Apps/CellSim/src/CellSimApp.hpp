#pragma once

#include "Engine.hpp"
#include "CellSimInterface.hpp"

namespace CS
{
    class CellSimApp final : public Engine::App
    {
        private:
            const float                     _nearPlane     = 0.1f;
            const float                     _farPlane      = 1024.0f;
            const float                     _camStartYaw   = 38.0f;
            const float                     _camStartPitch = -29.0f;
            const glm::vec3                 _lightPos      = glm::vec3(565.0f, 170.0f, 455.0f);
            const glm::vec3                 _lightTarget   = glm::vec3(515.0f, 40.0f, 505.0f);
            const glm::vec3                 _lightCol      = glm::vec3(1.0f, 1.0f, 1.0f);
            const glm::vec3                 _camStartPos   = glm::vec3(350.0f, 125.0f, 415.0f);

            Engine::SceneRenderer*             _sceneRenderer  = nullptr;
            Engine::CellRenderer*              _cellRenderer   = nullptr;
            Engine::ShadowRenderer*            _shadowRenderer = nullptr;
            Engine::SpriteRenderer*            _spriteRenderer = nullptr;
            Engine::Scope<CellSimInterface>    _interface;

            void           LoadResources() final;
            Engine::uint32 InitModules()   final;
            void           AddObjects();
            void           AddSprites();

        public:
            CellSimApp();
            ~CellSimApp() final;
            void Update() final;
    };
}