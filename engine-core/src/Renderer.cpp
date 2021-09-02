#include "Renderer.hpp"

namespace Core
{
    // ----- Public -----

    Renderer::Renderer(Camera* camera)
        :   _orthoProjection(glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, -1.0f, 1.0f)),
            _perspProjection(glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 500.0f)),
            _camera(camera),
            _cubemap(nullptr),
            _drawcalls(0),
            _drawnVertices(0),
            _renderPasses(0)
    {

    }

    void Renderer::PrepareFrame()
    {
        //Reset render stats for current frame
        _drawcalls = 0;
        _drawnVertices = 0;
        _renderPasses = 0;
    }

    void Renderer::Submit(const Sprite* sprite)
    {
        _spriteBuffer.push_back(sprite);
    }

    void Renderer::Submit(const Model* model)
    {
        _modelBuffer.push_back(model);
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
            _drawnVertices += model->Draw(modelShader, _perspProjection, _camera->GetViewMatrix(), _camera->GetPosition(), lightProjection);
            _drawcalls++;
        }

        //Increase render pass counter
        _renderPasses++;
    }

    void Renderer::FlushSprites()
    {
        //Render sprites
        for(const auto& sprite : _spriteBuffer)
        {
            _drawnVertices += sprite->Draw(_orthoProjection);
            _drawcalls++;
        }
    }

    void Renderer::FlushCubemap()
    {
        //Render cubemap last (if it exists)
        if(_cubemap)
        {
            _drawnVertices += _cubemap->Draw(_perspProjection, _camera->GetViewMatrix());
            _drawcalls++;
        }
    }

    void Renderer::FlushEverything(Shader* modelShader, const glm::mat4& lightProjection)
    {
        FlushModels(modelShader, lightProjection);
        FlushSprites();
        FlushCubemap();
    }

    uint32 Renderer::GetDrawcalls() const
    {
        return _drawcalls;
    }

    uint32 Renderer::GetDrawnVertices() const
    {
        return _drawnVertices;
    }

    uint32 Renderer::GetRenderPasses() const
    {
        return _renderPasses;
    }
}