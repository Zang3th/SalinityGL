#include "Renderer.hpp"

namespace Core
{
    // ----- Private -----

    const Cubemap*  Renderer::_cubemap;
    Camera*         Renderer::_camera;

    // ----- Public -----

    void Renderer::Init(Camera* camera)
    {
        _camera = camera;
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

    void Renderer::Submit(const Model* model, bool producesShadows)
    {
        if(producesShadows)
            _shadowModelBuffer.push_back(model);
        else
            _modelBuffer.push_back(model);
    }

    void Renderer::Submit(const Sprite* sprite)
    {
        _spriteBuffer.push_back(sprite);
    }   

    void Renderer::Submit(const Cubemap* cubemap)
    {
        _cubemap = cubemap;
    }

    void Renderer::FlushModels(Shader* modelShader, const glm::mat4& lightProjection)
    {
        //Check for Wireframe-Mode
        if(WireframeRendering){
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));}
        else{
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));}

        //Render models
        for(const auto& model : _modelBuffer)
        {
            _drawnVertices += model->DrawModel(modelShader, _perspProjection, _camera->GetViewMatrix(), _camera->GetPosition(), lightProjection);
            _drawcalls++;
        }

        //Increase render pass counter
        _modelRenderPasses++;
    }

    void Renderer::FlushShadowModels(Shader* modelShader, const glm::mat4& lightProjection)
    {
        //Check for Wireframe-Mode
        if(WireframeRendering){
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));}
        else{
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));}

        //Render models that cast shadows
        for(const auto& model : _shadowModelBuffer)
        {
            _drawnVertices += model->DrawModel(modelShader, _perspProjection, _camera->GetViewMatrix(), _camera->GetPosition(), lightProjection);
            _drawcalls++;
        }

        //Increase render pass counter
        _modelRenderPasses++;
    }

    void Renderer::FlushAllModels(Shader* modelShader, const glm::mat4& lightProjection)
    {
        FlushShadowModels(modelShader, lightProjection);
        FlushModels(modelShader, lightProjection);
    }

    void Renderer::RenderModel(const Model* model, Shader* modelShader, const glm::mat4& lightProjection)
    {
        //Check for Wireframe-Mode
        if(WireframeRendering){
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));}
        else{
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));}

        _drawnVertices += model->DrawModel(modelShader, _perspProjection, _camera->GetViewMatrix(), _camera->GetPosition(), lightProjection);
        _drawcalls++;

        //Increase render pass counter
        _modelRenderPasses++;
    }

    void Renderer::RenderWaterModel(const Model* model, Shader* modelShader)
    {
        //Check for Wireframe-Mode
        if(WireframeRendering){
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));}
        else{
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));}

        _drawnVertices += model->DrawWaterModel(modelShader, _perspProjection, _camera->GetViewMatrix(), _camera->GetPosition());
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
            _drawnVertices += _cubemap->Draw(_perspProjection, _camera->GetViewMatrix());
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