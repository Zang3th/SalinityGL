#pragma once

#include "Renderer.hpp"
#include "glm.hpp"
#include "Types.hpp"
#include "Cubemap.hpp"
#include "ResourceManager.hpp"
#include "RenderManager.hpp"
#include "GLRenderSettings.hpp"
#include "AppSettings.hpp"
#include "Camera3D.hpp"

#include <array>
#include <string>

namespace Engine
{
    class SceneRenderer final : public Renderer
    {
        friend class RenderManager;

        private:
            float     _nearPlane, _farPlane;
            glm::vec3 _lightPos,  _lightCol;
            glm::mat4 _perspProjection;

            Scope<Cubemap> _cubemap;

            SceneRenderer(float nearPlane, float farPlane, glm::vec3 lightPos, glm::vec3 lightCol);
            void FlushCubemap();

        public:
            void Flush() final;
            void AddCubemap(const std::array<const char*, 6>& faces, const std::string& shader);
    };
}