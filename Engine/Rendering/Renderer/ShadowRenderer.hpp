#pragma once

#include "FrameBuffer.hpp"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "ResourceManager.hpp"
#include "Renderer.hpp"
#include "GLRenderSettings.hpp"
#include "SceneRenderer.hpp"

namespace Engine
{
    class ShadowRenderer final : public Renderer
    {
        friend class RenderManager;

        private:
            glm::mat4           _orthoProjection, _lightView, _lightProjection;
            Scope<FrameBuffer>  _fbo;
            uint32              _shadowWidth;
            uint32              _shadowHeight;
            Shader*             _shadowShader;

            ShadowRenderer(uint32 width, uint32 height, const glm::vec3& lightPos, Shader* shader);
            
            void StartFrame();
            void EndFrame();

        public:
            void Flush(Renderer* sceneRenderer) final;
            [[nodiscard]] Texture*  GetDepthTexture()    const;
            [[nodiscard]] glm::mat4 GetLightProjection() const;
    };
}