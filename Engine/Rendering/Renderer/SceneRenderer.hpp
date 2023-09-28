#pragma once

#include "Renderer.hpp"
#include "glm.hpp"
#include "Types.hpp"
#include "Cubemap.hpp"
#include "ResourceManager.hpp"
#include "GLRenderSettings.hpp"
#include "Camera3D.hpp"
#include "Model.hpp"
#include "Heightmap.hpp"
#include "MeshCreator.hpp"
#include "ShadowRenderer.hpp"
#include "Window.hpp"

#include <array>
#include <string>
#include <optional>

namespace Engine
{
    class SceneRenderer final : public Renderer
    {
        friend class RenderManager;

        private:
            float     _nearPlane, _farPlane, _moveFactor, _waveSpeed;
            glm::vec3 _lightPos,  _lightCol;
            glm::mat4 _perspProj, _lightProj;

            Scope<Cubemap>      _cubemap;
            std::vector<Model*> _modelStorage;
            Model              *_terrainModel, *_waterModel, *_planeModel;
            Shader             *_terrainShader, *_modelShader, *_waterShader;

            SceneRenderer(float nearPlane, float farPlane, const glm::vec3& lightPos, const glm::vec3& lightCol);
            ~SceneRenderer() final;

            void FlushModel(Model* model, Shader* shader);
            void FlushWater();
            void UpdateMoveFactor();

        public:
            void Flush(Renderer* renderer) final;
            void FlushModels(Shader* shader);
            void FlushCubemap();
            void FlushTerrain();
            void FlushPlane();

            void AddLightProjection(const glm::mat4& lightProj);
            void AddCubemap(const std::array<const char*, 6>& faces, const std::string& shader);
            void AddTerrain
            (
                uint32 x, uint32 z, float tileSize, const glm::vec3& position, Texture* depthTexture,
                const std::string& texture, const std::string& colormap, const std::string& heightmap
            );
            void AddObject
            (
                float size, const glm::vec3& rotation, const glm::vec3& position,
                Texture* depthTexture, const std::string& objName, const std::string& objFilepath
            );
            void AddWater
            (
                uint32 x, uint32 z, float tileSize, const glm::vec3& position,
                float waveSpeed, const std::string& dudvMap, const std::string& normalMap,
                Texture* reflectTex, Texture* refractTex, Texture* refractDepthTex
            );
            void AddPlane
            (
                    uint32 x, uint32 z, float tileSize, const glm::vec3& position,
                    Texture* depthTexture, const std::optional<const std::string>& texture
            );

            void SetTerrainShader(const std::string& terrainShader);
            void SetModelShader(const std::string& modelShader);
            void SetWaterShader(const std::string& waterShader);
            [[nodiscard]] Shader* GetTerrainShader();
            [[nodiscard]] Shader* GetModelShader();
            [[nodiscard]] Shader* GetWaterShader();
            [[nodiscard]] glm::mat4 GetProjMatrix();
    };
}