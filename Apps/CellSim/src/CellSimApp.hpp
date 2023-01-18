#pragma once

#include "Engine.hpp"
#include "CellSimInterface.hpp"

namespace CS
{
    class CellSimApp final : public Engine::App
    {
        private:
            const float                     _nearPlane = 2.0f;
            const float                     _farPlane  = 300.0f;
            const glm::vec3                 _lightPos  = glm::vec3(150.0f, 100.0f, -30.0f);
            const glm::vec3                 _lightCol  = glm::vec3(1.0f, 1.0f, 1.0f);

            Engine::SceneRenderer*          _sceneRenderer  = nullptr;
            Engine::ShadowRenderer*         _shadowRenderer = nullptr;
            Engine::Scope<CellSimInterface> _interface;

            void LoadResources() final;
            void InitModules()   final;
            void AddObjects();

        public:
            CellSimApp();
            ~CellSimApp() final;
            void Update() final;
    };
}