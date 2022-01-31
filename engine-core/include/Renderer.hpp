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
            static const Cubemap*   _cubemap;
            static Camera*          _camera;
            inline static uint32    _drawcalls              = uint32();
            inline static uint32    _drawnVertices          = uint32();
            inline static uint32    _modelRenderPasses      = uint32();
            inline static uint32    _spriteRenderPasses     = uint32();
            inline static uint32    _cubemapRenderPasses    = uint32();
            inline static glm::mat4 _orthoProjection = glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, -1.0f, 1.0f);
            inline static glm::mat4 _perspProjection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 500.0f);
            inline static std::vector<const Model*>  _modelBuffer = std::vector<const Model*>();
            inline static std::vector<const Model*>  _shadowModelBuffer = std::vector<const Model*>();
            inline static std::vector<const Sprite*> _spriteBuffer = std::vector<const Sprite*>();

        public:
            Renderer() = delete;
            static void Init(Camera* camera);
            static void PrepareFrame();
            static void Submit(const Model* model, bool producesShadows);
            static void Submit(const Sprite* sprite);
            static void Submit(const Cubemap* cubemap);
            static void FlushModels(Shader* modelShader, const glm::mat4& lightProjection);
            static void FlushShadowModels(Shader* modelShader, const glm::mat4& lightProjection);
            static void FlushAllModels(Shader* modelShader, const glm::mat4& lightProjection);
            static void RenderModel(const Model* model, Shader* modelShader, const glm::mat4& lightProjection);
            static void RenderWaterModel(const Model* model, Shader* modelShader);
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