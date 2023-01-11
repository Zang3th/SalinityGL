#include "RenderManager.hpp"

namespace Engine
{
    // ----- Public -----

    void RenderManager::Init()
    {
        _rendererStorage.clear();
        _rendererStorage.reserve(5);

        //OpenGL-Rendersettings
        GLRenderSettings::EnableMultisample();
        GLRenderSettings::EnableDepthtest();
        GLRenderSettings::SetDepthFunc(GL_LEQUAL);
        GLRenderSettings::EnableBlending();
        GLRenderSettings::SetBlendFunc(GL_ONE_MINUS_SRC_ALPHA);
    }

    void RenderManager::CleanUp()
    {
        for(auto const& renderer : _rendererStorage)
            delete renderer;
    }

    void RenderManager::PrepareFrame()
    {
        APP_SETTINGS.renderStats.Reset();
        GLRenderSettings::ClearBuffers();
    }

    SceneRenderer* RenderManager::AddScene(const float nearPlane, const float farPlane, const glm::vec3& lightPos, const glm::vec3& lightCol)
    {
        _sceneRenderer = new SceneRenderer(nearPlane, farPlane, lightPos, lightCol);
        _rendererStorage.push_back(_sceneRenderer);

        return _sceneRenderer;
    }

    ShadowRenderer* RenderManager::AddShadows(const uint32 resolution, const glm::vec3& lightPos, const std::string& shader)
    {
        _shadowRenderer = new ShadowRenderer(resolution, resolution, lightPos, ResourceManager::GetShader(shader));
        _rendererStorage.push_back(_shadowRenderer);
        _sceneRenderer->AddLightProjection(_shadowRenderer->GetLightProjection());

        return _shadowRenderer;
    }

    SpriteRenderer* RenderManager::AddSprites()
    {
        _spriteRenderer = new SpriteRenderer();
        _rendererStorage.push_back(_spriteRenderer);

        return _spriteRenderer;
    }

    WaterRenderer* RenderManager::AddWater()
    {
        _waterRenderer = new WaterRenderer();
        _rendererStorage.push_back(_waterRenderer);

        return _waterRenderer;
    }

    ParticleRenderer* RenderManager::AddParticles
    (
        glm::vec3 position, uint32 count, float size, float speed,
        float gravityComplient, float lifeLength, float respawnThreshold,
        const std::string& textureAtlas, const std::string& shader
    )
    {
        _particleRenderer = new ParticleRenderer(position, count, size, speed, gravityComplient, lifeLength,
                                                 respawnThreshold, ResourceManager::GetTexture(textureAtlas),
                                                 ResourceManager::GetShader(shader));
        _rendererStorage.push_back(_particleRenderer);

        return _particleRenderer;
    }

    void RenderManager::RenderScene()
    {
        _sceneRenderer->Flush(nullptr);
    }

    void RenderManager::RenderShadows()
    {
        _shadowRenderer->Flush((Renderer*)_sceneRenderer);
    }

    void RenderManager::RenderSprites()
    {
        _spriteRenderer->Flush(nullptr);
    }

    void RenderManager::RenderWater()
    {
        _waterRenderer->Flush((Renderer*)_sceneRenderer);
    }

    void RenderManager::RenderParticles()
    {
        _particleRenderer->Flush((Renderer*)_sceneRenderer);
    }
}