#pragma once

#include "FrameBuffer.hpp"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Shader.hpp"
#include "GLTexture.hpp"
#include "ResourceManager.hpp"
#include "Renderer.hpp"
#include "GLRenderSettings.hpp"
#include "SceneRenderer.hpp"

namespace Engine
{
    class ShadowRenderer final : public Renderer
    {
        private:
            glm::mat4           _lightView, _lightProjection;
            Scope<FrameBuffer>  _fbo;
            uint32              _shadowWidth;
            uint32              _shadowHeight;
            Shader*             _shadowShader;

            void StartFrame();
            void EndFrame();

        public:
            ShadowRenderer(uint32 width, uint32 height, const glm::mat4& orthoProj, const std::string& shader);

            void Flush(Renderer* sceneRenderer) override;

            [[nodiscard]] GLTexture* GetDepthTexture() const;
            [[nodiscard]] glm::mat4  GetLightProjection() const;
    };
}
