#pragma once

#include "Renderer.hpp"
#include "glm.hpp"
#include "Types.hpp"
#include "Cubemap.hpp"
#include "ResourceManager.hpp"
#include "GLRenderSettings.hpp"
#include "AppSettings.hpp"
#include "Camera3D.hpp"
#include "Model.hpp"
#include "ModelManager.hpp"

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
            glm::mat4 _perspProj;

            Scope<Cubemap>      _cubemap;
            std::vector<Model*> _modelStorage;
            Model*              _terrainModel;

            Shader*             _terrainShader;

            SceneRenderer(float nearPlane, float farPlane, glm::vec3 lightPos, glm::vec3 lightCol);
            ~SceneRenderer() final;
            void FlushCubemap();
            void FlushTerrain();

        public:
            void Flush() final;
            void AddCubemap(const std::array<const char*, 6>& faces, const std::string& shader);
            void AddTerrain
            (
                uint32             x,
                uint32             z,
                float              tileSize,
                glm::vec3          position,
                ShadowRenderer*    shadowRenderer,
                const std::string& heightmapFilepath,
                const std::string& texture,
                const std::string& colormap,
                const std::string& shader
            );
    };
}