#include "Renderer.hpp"

namespace Core
{
    // ----- Public -----

    void Renderer::Init(glm::mat4 lightProjection)
    {
        _orthoProjection = glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, -1.0f, 1.0f);
        _perspProjection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 2.0f, 300.0f);
        //Nearplane of the _perspProjection has to be this big, otherwise the z-Buffer in the water rendering refraction depth pass is bugged

        _lightProjection = lightProjection;
    }

    void Renderer::PrepareFrame()
    {
        //Reset render stats for current frame
        _drawcalls              = 0;
        _drawnVertices          = 0;
        _modelRenderPasses      = 0;
        _spriteRenderPasses     = 0;
        _cubemapRenderPasses    = 0;
    }

    void Renderer::Submit(Model* model, bool producesShadows)
    {
        if(producesShadows)
            _shadowModelBuffer.push_back(model);
        else
            _modelBuffer.push_back(model);
    }

    void Renderer::Submit(Sprite* sprite)
    {
        _spriteBuffer.push_back(sprite);
    }   

    void Renderer::Submit(Cubemap* cubemap)
    {
        _cubemap = cubemap;
    }

    void Renderer::SubmitTerrain(Model* terrain)
    {
        _terrainModel = terrain;
    }

    void Renderer::SubmitWater(Model* water)
    {
        _waterModel = water;
    }

    void Renderer::FlushModelBuffer(Shader* modelShader)
    {
        //Check for Wireframe-Mode
        if(WIREFRAME_RENDERING){
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));}
        else{
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));}

        //Render models
        for(const auto& model : _modelBuffer)
        {
            _drawnVertices += model->DrawModel(modelShader, _perspProjection, Camera::GetViewMatrix(), Camera::GetPosition(), _lightProjection);
            _drawcalls++;
        }

        //Increase render pass counter
        _modelRenderPasses++;
    }

    void Renderer::FlushShadowModelBuffer(Shader* modelShader)
    {
        //Check for Wireframe-Mode
        if(WIREFRAME_RENDERING){
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));}
        else{
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));}

        //Render models that cast shadows
        for(const auto& model : _shadowModelBuffer)
        {
            _drawnVertices += model->DrawModel(modelShader, _perspProjection, Camera::GetViewMatrix(), Camera::GetPosition(), _lightProjection);
            _drawcalls++;
        }

        //Increase render pass counter
        _modelRenderPasses++;
    }

    void Renderer::FlushAllModelBuffers(Shader* modelShader)
    {
        FlushModelBuffer(modelShader);
        FlushShadowModelBuffer(modelShader);
    }

    void Renderer::FlushTerrainModel(Shader* modelShader)
    {
        //Check for Wireframe-Mode
        if(WIREFRAME_RENDERING){
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));}
        else{
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));}

        _drawnVertices += _terrainModel->DrawTerrainModel(modelShader, _perspProjection, Camera::GetViewMatrix(), Camera::GetPosition(), _lightProjection);
        _drawcalls++;

        //Increase render pass counter
        _modelRenderPasses++;
    }

    void Renderer::FlushWaterModel(Shader* modelShader, float moveFactor)
    {
        //Check for Wireframe-Mode
        if(WIREFRAME_RENDERING){
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));}
        else{
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));}

        _drawnVertices += _waterModel->DrawWaterModel(modelShader, _perspProjection, Camera::GetViewMatrix(), Camera::GetPosition(), moveFactor);
        _drawcalls++;

        //Increase render pass counter
        _modelRenderPasses++;
    }

    void Renderer::FlushSprites()
    {
        GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

        //Render sprites
        for(const auto& sprite : _spriteBuffer)
        {
            _drawnVertices += sprite->Draw(_orthoProjection);
            _drawcalls++;
        }

        //Increase render pass counter
        _spriteRenderPasses++;
    }

    void Renderer::FlushCubemap()
    {
        //Render cubemap last (if it exists)
        if(_cubemap)
        {
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
            _drawnVertices += _cubemap->Draw(_perspProjection, Camera::GetViewMatrix());
            _drawcalls++;

            //Increase render pass counter
            _cubemapRenderPasses++;
        }
    }

    void Renderer::ClearBuffers()
    {
        GLCall(glClearColor(0.0, 0.0, 0.0, 1.0));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    uint32 Renderer::GetDrawcalls()
    {
        return _drawcalls;
    }

    uint32 Renderer::GetDrawnVertices()
    {
        return _drawnVertices;
    }

    uint32 Renderer::GetModelRenderPasses()
    {
        return _modelRenderPasses;
    }

    uint32 Renderer::GetSpriteRenderPasses()
    {
        return _spriteRenderPasses;
    }

    uint32 Renderer::GetCubemapRenderPasses()
    {
        return _cubemapRenderPasses;
    }
}