#pragma once

#include "Camera.hpp"
#include "glm.hpp"
#include "Sprite.hpp"
#include "Model.hpp"
#include "CoreDefines.hpp"
#include "ErrorManager.hpp"
#include "Cubemap.hpp"
#include "Shader.hpp"
#include "ParticleRenderer.hpp"

#include <vector>

namespace Engine
{
    class Renderer
    {
        private:
            inline static Cubemap                   *_cubemap                   = nullptr;
            inline static Model                     *_terrainModel              = nullptr;
            inline static Model                     *_waterModel                = nullptr;
            inline static ParticleRenderer          *_particleRenderer          = nullptr;
            inline static float                      _nearPlane                 = float();
            inline static float                      _farPlane                  = float();
            inline static uint32                     _drawcalls                 = uint32();
            inline static uint32                     _drawnVertices             = uint32();
            inline static uint32                     _modelRenderPasses         = uint32();
            inline static uint32                     _terrainRenderPasses       = uint32();
            inline static uint32                     _waterRenderPasses         = uint32();
            inline static uint32                     _particleRenderPasses      = uint32();
            inline static uint32                     _spriteRenderPasses        = uint32();
            inline static uint32                     _cubemapRenderPasses       = uint32();
            inline static glm::vec3                  _lightPosition             = glm::vec3();
            inline static glm::vec3                  _lightColor                = glm::vec3();
            inline static glm::mat4                  _orthoProjection           = glm::mat4();
            inline static glm::mat4                  _perspProjection           = glm::mat4();
            inline static glm::mat4                  _lightProjection           = glm::mat4();
            inline static std::vector<const Model*>  _modelBuffer               = std::vector<const Model*>();
            inline static std::vector<const Sprite*> _spriteBuffer              = std::vector<const Sprite*>();

            static uint32 DrawModel(Shader* modelShader, const Model* model);
            static uint32 DrawTerrain(Shader* terrainShader);
            static uint32 DrawWater(Shader* waterShader, float moveFactor);
            static uint32 DrawParticles(Shader* particleShader);

        public:
            Renderer() = delete;
            static void Init(float nearPlane, float farPlane, glm::vec3 lightPos, glm::vec3 lightColor, glm::mat4 lightProjection);
            static void PrepareFrame();
            static void Submit(Model* model);
            static void Submit(Sprite* sprite);
            static void Submit(Cubemap* cubemap);
            static void Submit(ParticleRenderer* particleRenderer);
            static void SubmitTerrain(Model* terrain);
            static void SubmitWater(Model* water);

            static void FlushModelBuffer(Shader* modelShader);
            static void FlushTerrain(Shader* terrainShader);
            static void FlushWater(Shader* waterShader, float moveFactor);
            static void FlushParticleRenderer(Shader* particleShader);
            static void FlushSprites();
            static void FlushCubemap();

            static void ClearBuffers();

            static uint32 GetDrawcalls();
            static uint32 GetDrawnVertices();
            static uint32 GetModelRenderPasses();
            static uint32 GetTerrainRenderPasses();
            static uint32 GetWaterRenderPasses();
            static uint32 GetParticleRenderPasses();
            static uint32 GetSpriteRenderPasses();
            static uint32 GetCubemapRenderPasses();
    };
}