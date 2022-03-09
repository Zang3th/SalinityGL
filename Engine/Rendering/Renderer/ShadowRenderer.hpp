#pragma once

#include "FrameBuffer.hpp"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "ResourceManager.hpp"
#include "Renderer.hpp"

namespace Engine
{
    class ShadowRenderer
    {
        private:
            glm::mat4           _orthoProjection, _lightView, _lightProjection;
            Scope<FrameBuffer>  _fbo;
            uint32              _shadowWidth;
            uint32              _shadowHeight;

            void StartFrame();
            void EndFrame();

        public:
            ShadowRenderer(uint32 width, uint32 height, glm::vec3 lightPos);
            void RenderToFramebuffer(Shader* shadowShader);
            [[nodiscard]] Texture* GetDepthTexture() const;
            [[nodiscard]] glm::mat4 GetLightProjection() const;
    };
}