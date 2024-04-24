#include "RenderManager.hpp"

namespace Engine
{
    // ----- Public -----

    void RenderManager::Init()
    {
        _rendererStorage.clear();
        _rendererStorage.reserve(10);

        //OpenGL-Rendersettings
        GLRenderSettings::EnableMultisample();
        GLRenderSettings::EnableDepthtest();
        GLRenderSettings::SetDepthFunc(GL_LEQUAL);
        GLRenderSettings::EnableBlending();
        GLRenderSettings::SetBlendFunc(GL_ONE_MINUS_SRC_ALPHA);
        GLRenderSettings::EnableCulling();
        GLRenderSettings::SetCullFace(GL_BACK);

        _initialized = true;
    }

    void RenderManager::CleanUp()
    {
        for(auto const& renderer : _rendererStorage)
            delete renderer;
    }

    void RenderManager::PrepareFrame()
    {
        RenderStatistics::Reset();
        GLRenderSettings::ClearBuffers();
    }

    bool RenderManager::GetInitStatus()
    {
        return _initialized;
    }

    SceneRenderer* RenderManager::AddSceneRenderer()
    {
        _sceneRenderer = new SceneRenderer();
        _rendererStorage.push_back(_sceneRenderer);

        return _sceneRenderer;
    }

    ShadowRenderer* RenderManager::AddShadowRenderer(uint32 resolution, const glm::mat4& orthoProj, const std::string& shader)
    {
        _shadowRenderer = new ShadowRenderer(resolution, resolution, orthoProj, ResourceManager::GetShader(shader));
        _rendererStorage.push_back(_shadowRenderer);
        _sceneRenderer->AddLightProjection(_shadowRenderer->GetLightProjection());

        return _shadowRenderer;
    }

    SpriteRenderer* RenderManager::AddSpriteRenderer()
    {
        _spriteRenderer = new SpriteRenderer();
        _rendererStorage.push_back(_spriteRenderer);

        return _spriteRenderer;
    }

    WaterRenderer* RenderManager::AddWaterRenderer()
    {
        _waterRenderer = new WaterRenderer();
        _rendererStorage.push_back(_waterRenderer);

        return _waterRenderer;
    }

    ParticleRenderer* RenderManager::AddParticleRenderer
    (
        uint32 count, float size, float speed, float gravityCompliance, float lifeLength,
        float respawnThreshold, const std::string& textureAtlas, const std::string& shader, const glm::vec3& position
    )
    {
        _particleRenderer = new ParticleRenderer
        (
            count,
            size,
            speed,
            gravityCompliance,
            lifeLength,
            respawnThreshold,
            ResourceManager::GetTexture(textureAtlas),
            ResourceManager::GetShader(shader),
            position
        );
        _rendererStorage.push_back(_particleRenderer);

        return _particleRenderer;
    }

    CellRenderer* RenderManager::AddCellRenderer(const std::string& shader, const glm::vec3& worldSpawnPos)
    {
        _cellRenderer = new CellRenderer(ResourceManager::GetShader(shader), worldSpawnPos);
        _rendererStorage.push_back(_cellRenderer);

        return _cellRenderer;
    }

    PixelRenderer* RenderManager::AddPixelRenderer
    (
        const uint32 width, const uint32 height, const uint32 pxSize,
        const std::string& bgTexture, const std::string& shader
    )
    {
        _pixelRenderer = new PixelRenderer(width, height, pxSize, bgTexture, shader);
        _rendererStorage.push_back(_pixelRenderer);

        return _pixelRenderer;
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

    void RenderManager::RenderCells()
    {
        _cellRenderer->Flush((Renderer*)_sceneRenderer);
    }

    void RenderManager::RenderPixels()
    {
        _pixelRenderer->Flush(nullptr);
    }
}