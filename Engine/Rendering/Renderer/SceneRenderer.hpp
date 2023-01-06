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
#include "Heightmap.hpp"
#include "MeshCreator.hpp"
#include "ShadowRenderer.hpp"

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
            glm::mat4 _perspProj, _lightProj;

            Scope<Cubemap>      _cubemap;
            std::vector<Model*> _modelStorage;
            Model*              _terrainModel;
            Shader             *_terrainShader, *_modelShader, *_waterShader;

            SceneRenderer(float nearPlane, float farPlane, const glm::vec3& lightPos, const glm::vec3& lightCol);
            ~SceneRenderer() final;
            void FlushCubemap();
            void FlushTerrain();
            void FlushModel(Model* model, Shader* shader);

        public:
            void SetDefaultShaders(const std::string& terrainShader, const std::string& modelShader, const std::string& waterShader);
            void AddLightProjection(const glm::mat4& lightProj);
            void Flush(Renderer* renderer) final;
            void FlushModels(Shader* shader);
            void AddCubemap(const std::array<const char*, 6>& faces, const std::string& shader);
            void AddTerrain
            (
                uint32 x, uint32 z, float tileSize, const glm::vec3& position, Texture* depthTexture,
                const std::string& heightmapFilepath, const std::string& texture, const std::string& colormap
            );
            void AddObject
            (
                float size, const glm::vec3& rotation, const glm::vec3& position,
                Texture* depthTexture, const std::string& objName, const std::string& objFilepath
            );
    };
}