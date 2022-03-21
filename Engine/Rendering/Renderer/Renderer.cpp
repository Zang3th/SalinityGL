#include "Renderer.hpp"

namespace Engine
{
    // ----- Private -----

    uint32 Renderer::DrawModel(Shader* modelShader, const Model* model)
    {
        modelShader->Bind();
        bool gotNormalMap = model->GotNormalMap();

        //Set uniforms
        modelShader->SetUniformMat4f("view", Camera::GetViewMatrix());
        modelShader->SetUniformMat4f("model", model->GetModelMatrix());
        modelShader->SetUniformMat4f("projection", _perspProjection);
        modelShader->SetUniformVec3f("viewPos", Camera::GetPosition());
        modelShader->SetUniformVec3f("lightPos", _lightPosition);
        modelShader->SetUniformVec3f("lightColor", _lightColor);
        modelShader->SetUniformMat4f("lightProjection", _lightProjection);
        modelShader->SetUniform1i("diffuseTexture", 0);
        modelShader->SetUniform1i("normalMap", 1);
        modelShader->SetUniform1i("shadowMap", 2);
        modelShader->SetUniform1i("gotNormalMap", gotNormalMap);

        //Get textures
        Texture* texture1 = model->GetTexture1();
        Texture* texture2 = model->GetTexture2();
        Texture* texture3 = model->GetTexture3();

        //Diffuse texture
        if(texture1)
            texture1->BindToSlot(0);

        //Normal map
        if(texture2)
            texture2->BindToSlot(1);

        //Shadow map
        if(texture3)
            texture3->BindToSlot(2);

        //Get rendering data
        VertexArray* vao    = model->GetVAO();
        uint32 verticeCount = model->GetVerticeCount();

        //Render model
        vao->Bind();
        GLCall(glDrawElements(GL_TRIANGLES, verticeCount, GL_UNSIGNED_INT, nullptr));
        vao->Unbind();

        modelShader->Unbind();

        //Return rendered vertices
        return verticeCount;
    }

    uint32 Renderer::DrawTerrain(Shader* terrainShader)
    {
        terrainShader->Bind();

        //Set uniforms
        terrainShader->SetUniformMat4f("view", Camera::GetViewMatrix());
        terrainShader->SetUniformMat4f("model", _terrainModel->GetModelMatrix());
        terrainShader->SetUniformMat4f("projection", _perspProjection);
        terrainShader->SetUniformVec3f("viewPos", Camera::GetPosition());
        terrainShader->SetUniformVec3f("lightPos", _lightPosition);
        terrainShader->SetUniformVec3f("lightColor", _lightColor);
        terrainShader->SetUniformMat4f("lightProjection", _lightProjection);
        terrainShader->SetUniform1i("diffuseTexture", 0);
        terrainShader->SetUniform1i("colorMap", 1);
        terrainShader->SetUniform1i("shadowMap", 2);

        //Get textures
        Texture* texture1 = _terrainModel->GetTexture1();
        Texture* texture2 = _terrainModel->GetTexture2();
        Texture* texture3 = _terrainModel->GetTexture3();

        //Set diffuse texture
        if(texture1)
            texture1->BindToSlot(0);

        //Set normal map
        if(texture2)
            texture2->BindToSlot(1);

        //Set shadow map
        if(texture3)
            texture3->BindToSlot(2);

        //Get rendering data
        VertexArray* vao    = _terrainModel->GetVAO();
        uint32 verticeCount = _terrainModel->GetVerticeCount();

        //Render model
        vao->Bind();
        GLCall(glDrawElements(GL_TRIANGLES, verticeCount, GL_UNSIGNED_INT, nullptr));
        vao->Unbind();

        terrainShader->Unbind();

        //Return rendered vertices
        return verticeCount;
    }

    uint32 Renderer::DrawWater(Shader* waterShader, float moveFactor)
    {
        waterShader->Bind();

        //Set uniforms
        waterShader->SetUniformMat4f("view", Camera::GetViewMatrix());
        waterShader->SetUniformMat4f("model", _waterModel->GetModelMatrix());
        waterShader->SetUniformMat4f("projection", _perspProjection);
        waterShader->SetUniformVec3f("viewPos", Camera::GetPosition());
        waterShader->SetUniformVec3f("lightPos", _lightPosition);
        waterShader->SetUniformVec3f("lightColor", _lightColor);
        waterShader->SetUniform1i("reflectionTexture", 0);
        waterShader->SetUniform1i("refractionTexture", 1);
        waterShader->SetUniform1i("dudvMap", 2);
        waterShader->SetUniform1i("normalMap", 3);
        waterShader->SetUniform1i("depthMap", 4);
        waterShader->SetUniform1f("moveFactor", moveFactor);
        waterShader->SetUniform1f("nearPlane", _nearPlane);
        waterShader->SetUniform1f("farPlane", _farPlane);

        //Get textures
        Texture* texture1 = _waterModel->GetTexture1();
        Texture* texture2 = _waterModel->GetTexture2();
        Texture* texture3 = _waterModel->GetTexture3();
        Texture* texture4 = _waterModel->GetTexture4();
        Texture* texture5 = _waterModel->GetTexture5();

        //Reflection texture
        if(texture1)
            texture1->BindToSlot(0);

        //Refraction texture
        if(texture2)
            texture2->BindToSlot(1);

        //DuDvMap
        if(texture3)
            texture3->BindToSlot(2);

        //NormalMap
        if(texture4)
            texture4->BindToSlot(3);

        //DepthMap
        if(texture5)
            texture5->BindToSlot(4);

        //Get rendering data
        VertexArray* vao    = _waterModel->GetVAO();
        uint32 verticeCount = _waterModel->GetVerticeCount();

        //Render model
        vao->Bind();
        GLCall(glDrawElements(GL_TRIANGLES, verticeCount, GL_UNSIGNED_INT, nullptr));
        vao->Unbind();

        waterShader->Unbind();

        //Return rendered vertices
        return verticeCount;
    }

    // ----- Public -----

    void Renderer::Init(const float nearPlane, const float farPlane, const glm::vec3 lightPos, const glm::vec3 lightColor, const glm::mat4 lightProjection)
    {
        _nearPlane       = nearPlane;
        _farPlane        = farPlane;
        _lightPosition   = lightPos;
        _lightColor      = lightColor;
        _lightProjection = lightProjection;
        _orthoProjection = glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, -1.0f, 1.0f);

        //Nearplane of the _perspProjection has to be this big, otherwise the z-Buffer in the water rendering refraction depth pass is bugged
        _perspProjection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, _nearPlane, _farPlane);

        //OpenGL-Rendersettings
        GLRenderSettings::EnableMultisample();
        GLRenderSettings::EnableDepthtest();
        GLRenderSettings::SetDepthFunc(GL_LEQUAL);
        GLRenderSettings::EnableBlending();
        GLRenderSettings::SetBlendFunc(GL_ONE_MINUS_SRC_ALPHA);
    }

    void Renderer::PrepareFrame()
    {
        //Reset render stats for current frame
        _drawcalls                = 0;
        _drawnVertices            = 0;
        _modelRenderPasses        = 0;
        _terrainRenderPasses      = 0;
        _waterRenderPasses        = 0;
        _particleRenderPasses     = 0;
        _spriteRenderPasses       = 0;
        _cubemapRenderPasses      = 0;
    }

    void Renderer::Submit(Model* model)
    {
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

    void Renderer::Submit(ParticleRenderer* particleRenderer)
    {
        _particleRenderer = particleRenderer;
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
        if(WIREFRAME_RENDERING)
            GLRenderSettings::EnableWireframe();
        else
            GLRenderSettings::DisableWireframe();

        ///Draw models and update render stats
        for(auto model : _modelBuffer)
        {
            _drawnVertices += DrawModel(modelShader, model);
            _drawcalls++;
        }

        //Increase render pass counter
        _modelRenderPasses++;
    }

    void Renderer::FlushTerrain(Shader* terrainShader)
    {
        //Check for Wireframe-Mode
        if(WIREFRAME_RENDERING)
            GLRenderSettings::EnableWireframe();
        else
            GLRenderSettings::DisableWireframe();

        //Draw model and update render stats
        _drawnVertices += DrawTerrain(terrainShader);
        _drawcalls++;
        _terrainRenderPasses++;
    }

    void Renderer::FlushWater(Shader* waterShader, float moveFactor)
    {
        //Check for Wireframe-Mode
        if(WIREFRAME_RENDERING)
            GLRenderSettings::EnableWireframe();
        else
            GLRenderSettings::DisableWireframe();

        //Draw model and update render stats
        _drawnVertices += DrawWater(waterShader, moveFactor);
        _drawcalls++;
        _waterRenderPasses++;
    }

    void Renderer::FlushParticleRenderer()
    {
        GLRenderSettings::DisableDepthtest();

        //Check for Wireframe-Mode
        if(WIREFRAME_RENDERING)
            GLRenderSettings::EnableWireframe();
        else
            GLRenderSettings::DisableWireframe();

        //Draw particles and update render stats
        uint32 particleCount  = _particleRenderer->Render(_perspProjection);
        _drawnVertices       += particleCount * 4;
        _drawcalls           += particleCount;
        _particleRenderPasses++;

        GLRenderSettings::EnableDepthtest();
    }

    void Renderer::FlushSprites()
    {
        GLRenderSettings::DisableWireframe();

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
        //Render cubemap if it exists
        if(_cubemap)
        {
            GLRenderSettings::DisableWireframe();
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

    uint32 Renderer::GetTerrainRenderPasses()
    {
        return _terrainRenderPasses;
    }

    uint32 Renderer::GetWaterRenderPasses()
    {
        return _waterRenderPasses;
    }

    uint32 Renderer::GetParticleRenderPasses()
    {
        return _particleRenderPasses;
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