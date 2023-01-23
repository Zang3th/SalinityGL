#pragma once

#include "Engine.hpp"
#include "CellSimInterface.hpp"

namespace CS
{
    class CellSimApp final : public Engine::App
    {
        private:
            const float                     _nearPlane   = 0.1f;
            const float                     _farPlane    = 1024.0f;
            const glm::vec3                 _lightPos    = glm::vec3(560.0f, 197.5f, 460.0f);
            const glm::vec3                 _lightTarget = glm::vec3(500.0f, 0.0f, 524.0f);
            const glm::vec3                 _lightCol    = glm::vec3(1.0f, 1.0f, 1.0f);

            Engine::SceneRenderer*          _sceneRenderer  = nullptr;
            Engine::ShadowRenderer*         _shadowRenderer = nullptr;
            Engine::SpriteRenderer*         _spriteRenderer = nullptr;
            Engine::CellRenderer*           _cellRenderer   = nullptr;
            Engine::Scope<CellSimInterface> _interface;

            void LoadResources() final;
            void InitModules()   final;
            void AddObjects();
            void AddSprites();

        public:
            CellSimApp();
            ~CellSimApp() final;
            void Update() final;
    };
}