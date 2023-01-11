#pragma once

#include "Types.hpp"
#include "ErrorManager.hpp"
#include "glm.hpp"
#include "Renderer.hpp"
#include "SceneRenderer.hpp"
#include "SpriteRenderer.hpp"
#include "WaterRenderer.hpp"
#include "ParticleRenderer.hpp"

#include <vector>

namespace Engine
{
    class RenderManager
    {
        private:
            inline static std::vector<Renderer*> _rendererStorage  = std::vector<Renderer*>();
            inline static SceneRenderer*         _sceneRenderer    = nullptr;
            inline static ShadowRenderer*        _shadowRenderer   = nullptr;
            inline static SpriteRenderer*        _spriteRenderer   = nullptr;
            inline static WaterRenderer*         _waterRenderer    = nullptr;
            inline static ParticleRenderer*      _particleRenderer = nullptr;

        public:
            RenderManager() = delete;

            static void Init();
            static void CleanUp();
            static void PrepareFrame();

            static SceneRenderer*    AddScene(float nearPlane, float farPlane, const glm::vec3& lightPos, const glm::vec3& lightCol);
            static ShadowRenderer*   AddShadows(uint32 resolution, const glm::vec3& lightPos, const std::string& shader);
            static SpriteRenderer*   AddSprites();
            static WaterRenderer*    AddWater();
            static ParticleRenderer* AddParticles
            (
                glm::vec3 position, uint32 count, float size, float speed,
                float gravityComplient, float lifeLength, float respawnThreshold,
                const std::string& textureAtlas, const std::string& shader
            );

            static void RenderScene();
            static void RenderShadows();
            static void RenderSprites();
            static void RenderWater();
            static void RenderParticles();
    };
}