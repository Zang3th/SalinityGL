#pragma once

#include "Camera.hpp"
#include "glm.hpp"
#include "Sprite.hpp"
#include "Model.hpp"
#include "CoreDefines.hpp"
#include "ErrorManager.hpp"
#include "Cubemap.hpp"
#include "Shader.hpp"

#include <vector>

namespace Core
{
    class Renderer
    {
        private:
            inline static Cubemap                   *_cubemap               = nullptr;
            inline static Model                     *_terrainModel          = nullptr;
            inline static Model                     *_waterModel            = nullptr;
            inline static uint32                     _drawcalls             = uint32();
            inline static uint32                     _drawnVertices         = uint32();
            inline static uint32                     _modelRenderPasses     = uint32();
            inline static uint32                     _spriteRenderPasses    = uint32();
            inline static uint32                     _cubemapRenderPasses   = uint32();
            inline static glm::mat4                  _orthoProjection       = glm::mat4();
            inline static glm::mat4                  _perspProjection       = glm::mat4();
            inline static glm::mat4                  _lightProjection       = glm::mat4();
            inline static std::vector<const Model*>  _modelBuffer           = std::vector<const Model*>();
            inline static std::vector<const Model*>  _shadowModelBuffer     = std::vector<const Model*>();
            inline static std::vector<const Sprite*> _spriteBuffer          = std::vector<const Sprite*>();

        public:
            Renderer() = delete;
            static void Init(glm::mat4 lightProjection);
            static void PrepareFrame();
            static void Submit(Model* model, bool producesShadows);
            static void Submit(Sprite* sprite);
            static void Submit(Cubemap* cubemap);
            static void SubmitTerrain(Model* model);
            static void SubmitWater(Model* model);

            static void FlushModelBuffer(Shader* modelShader);
            static void FlushShadowModelBuffer(Shader* modelShader);
            static void FlushAllModelBuffers(Shader* modelShader);
            static void FlushTerrainModel(Shader* modelShader);
            static void FlushWaterModel(Shader* modelShader, float moveFactor);
            static void FlushSprites();
            static void FlushCubemap();

            static void ClearBuffers();

            static uint32 GetDrawcalls();
            static uint32 GetDrawnVertices();
            static uint32 GetModelRenderPasses();
            static uint32 GetSpriteRenderPasses();
            static uint32 GetCubemapRenderPasses();
    };
}