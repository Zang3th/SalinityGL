#pragma once

#include "FrameBuffer.hpp"
#include "Renderer.hpp"

namespace Core
{
    class WaterRenderer
    {
        private:
            Scope<FrameBuffer>  _reflectFBO, _refractFBO;
            const uint32        _reflectionWidth  = 800;
            const uint32        _reflectionHeight = 800;
            const uint32        _refractionWidth  = 800;
            const uint32        _refractionHeight = 800;
            const glm::vec4     _reflectionClipPlane = glm::vec4(0, 1, 0, -0.3);
            const glm::vec4     _refractionClipPlane = glm::vec4(0, -1, 0, 0.05);

            void InitReflectionFBO();
            void InitRefractionFBO();
            void StartReflectionFrame();
            void EndReflectionFrame();
            void StartRefractionFrame();
            void EndRefractionFrame();
            void RenderReflectionFrame(Shader* modelShader, glm::mat4 lightProjection);
            void RenderRefractionFrame(Shader* modelShader, glm::mat4 lightProjection);

        public:
            explicit WaterRenderer();
            void RenderToFramebuffer(Shader* modelShader, glm::mat4 lightProjection);
            [[nodiscard]] Texture* GetReflectTexture() const;
            [[nodiscard]] Texture* GetRefractTexture() const;
            [[nodiscard]] Texture* GetRefractDepthTexture() const;
    };
}