#include "SceneRenderer.hpp"

namespace Engine
{
    // ----- Private -----

    SceneRenderer::SceneRenderer(const float nearPlane, const float farPlane, const glm::vec3& lightPos, const glm::vec3& lightCol)
        : _nearPlane(nearPlane), _farPlane(farPlane), _moveFactor(0.0f), _waveSpeed(0.0f), _lightPos(lightPos), _lightCol(lightCol),
          _perspProj(glm::perspective(glm::radians(45.0f), Window::GetAspectRatio(), _nearPlane, _farPlane)), _lightProj(0.0f),
          _terrainModel(nullptr), _waterModel(nullptr), _terrainShader(nullptr), _modelShader(nullptr), _waterShader(nullptr)
    {
        Logger::Info("Created", __func__);
    }

    SceneRenderer::~SceneRenderer()
    {
        for(auto const& model : _modelStorage)
            delete model;

        delete _terrainModel;
        delete _waterModel;
    }

    void SceneRenderer::FlushModel(Model* model, Shader* shader)
    {
        //Bind shader
        shader->Bind();

        //Get textures and bind them
        uint8_t slot = 0;
        for(const auto& tex : *model->GetTextures())
        {
            tex->BindToSlot(slot++);
        }

        //Bind buffers and get vertice count
        model->BindBuffers();
        uint32 verticeCount = model->GetVerticeCount();

        //Set uniforms
        shader->SetUniformMat4f("view", Camera3D::GetViewMatrix());
        shader->SetUniformMat4f("model", model->GetModelMatrix());
        shader->SetUniformMat4f("projection", _perspProj);
        shader->SetUniformVec3f("viewPos", Camera3D::GetPosition());
        shader->SetUniformVec3f("lightPos", _lightPos);
        shader->SetUniformVec3f("lightColor", _lightCol);
        shader->SetUniformMat4f("lightProjection", _lightProj);
        shader->SetUniform1i("diffuseTexture", 0);
        shader->SetUniform1i("shadowMap", 1);
        shader->SetUniform1i("normalMap", 2);
        shader->SetUniform1i("gotNormalMap", model->GotNormalMap());

        //Render model
        GLCall(glDrawElements(GL_TRIANGLES, verticeCount, GL_UNSIGNED_INT, nullptr));

        //Unbind buffers
        model->UnbindBuffers();

        //Unbind textures
        for(const auto& tex : *model->GetTextures())
        {
            tex->Unbind();
        }

        //Unbind shader
        shader->Unbind();

        //Save stats
        APP_SETTINGS.renderStats.drawnVertices += verticeCount;
        APP_SETTINGS.renderStats.drawCalls++;
    }

    void SceneRenderer::FlushWater()
    {
        //Bind shader
        _waterShader->Bind();

        //Get textures and bind them
        uint8_t slot = 0;
        for(const auto& tex : *_waterModel->GetTextures())
        {
            tex->BindToSlot(slot++);
        }

        //Bind buffers and get vertice count
        _waterModel->BindBuffers();
        uint32 verticeCount = _waterModel->GetVerticeCount();

        //Set uniforms
        _waterShader->SetUniformMat4f("view", Camera3D::GetViewMatrix());
        _waterShader->SetUniformMat4f("model", _waterModel->GetModelMatrix());
        _waterShader->SetUniformMat4f("projection", _perspProj);
        _waterShader->SetUniformVec3f("viewPos", Camera3D::GetPosition());
        _waterShader->SetUniformVec3f("lightPos", _lightPos);
        _waterShader->SetUniformVec3f("lightColor", _lightCol);
        _waterShader->SetUniform1i("reflectionTexture", 0);
        _waterShader->SetUniform1i("refractionTexture", 1);
        _waterShader->SetUniform1i("dudvMap", 2);
        _waterShader->SetUniform1i("normalMap", 3);
        _waterShader->SetUniform1i("depthMap", 4);
        _waterShader->SetUniform1f("moveFactor", _moveFactor);
        _waterShader->SetUniform1f("nearPlane", _nearPlane);
        _waterShader->SetUniform1f("farPlane", _farPlane);

        //Render model
        GLCall(glDrawElements(GL_TRIANGLES, verticeCount, GL_UNSIGNED_INT, nullptr));

        //Unbind buffers
        _waterModel->UnbindBuffers();

        //Unbind textures
        for(const auto& tex : *_waterModel->GetTextures())
        {
            tex->Unbind();
        }

        //Unbind shader
        _waterShader->Unbind();

        //Save stats
        APP_SETTINGS.renderStats.drawnVertices += verticeCount;
        APP_SETTINGS.renderStats.drawCalls++;
        APP_SETTINGS.renderStats.waterPasses++;
    }

    void SceneRenderer::UpdateMoveFactor()
    {
        _moveFactor += _waveSpeed * (float)Window::GetDeltaTime();
        _moveFactor = (float)fmod(_moveFactor, 1.0f);
    }

    // ----- Public -----

    void SceneRenderer::Flush(Renderer* renderer)
    {
        //Check for Wireframe-Mode
        if(APP_SETTINGS.wireframeRendering)
            GLRenderSettings::EnableWireframe();
        else
            GLRenderSettings::DisableWireframe();

        //Modify water move factor
        UpdateMoveFactor();

        FlushTerrain();
        FlushModels(_modelShader);
        FlushWater();
        FlushCubemap();
    }

    void SceneRenderer::FlushModels(Shader* shader)
    {
        for(auto& model : _modelStorage)
        {
            FlushModel(model, shader);
        }

        APP_SETTINGS.renderStats.modelPasses++;
    }

    void SceneRenderer::FlushCubemap()
    {
        if(_cubemap)
        {
            GLRenderSettings::DisableWireframe();
            APP_SETTINGS.renderStats.drawnVertices += _cubemap->Draw(_perspProj, Camera3D::GetViewMatrix());
            APP_SETTINGS.renderStats.drawCalls++;
            APP_SETTINGS.renderStats.cubemapPasses++;
        }
    }

    void SceneRenderer::FlushTerrain()
    {
        //Bind shader
        _terrainShader->Bind();

        //Get textures and bind them
        uint8_t slot = 0;
        for(const auto& tex : *_terrainModel->GetTextures())
        {
            tex->BindToSlot(slot++);
        }

        //Bind buffers and get vertice count
        _terrainModel->BindBuffers();
        uint32 verticeCount = _terrainModel->GetVerticeCount();

        //Set uniforms
        _terrainShader->SetUniformMat4f("view", Camera3D::GetViewMatrix());
        _terrainShader->SetUniformMat4f("model", _terrainModel->GetModelMatrix());
        _terrainShader->SetUniformMat4f("projection", _perspProj);
        _terrainShader->SetUniformVec3f("viewPos", Camera3D::GetPosition());
        _terrainShader->SetUniformVec3f("lightPos", _lightPos);
        _terrainShader->SetUniformVec3f("lightColor", _lightCol);
        _terrainShader->SetUniformMat4f("lightProjection", _lightProj);
        _terrainShader->SetUniform1i("diffuseTexture", 0);
        _terrainShader->SetUniform1i("colorMap", 1);
        _terrainShader->SetUniform1i("shadowMap", 2);

        //Render model
        GLCall(glDrawElements(GL_TRIANGLES, verticeCount, GL_UNSIGNED_INT, nullptr));

        //Unbind buffers
        _terrainModel->UnbindBuffers();

        //Unbind textures
        for(const auto& tex : *_terrainModel->GetTextures())
        {
            tex->Unbind();
        }

        //Unbind shader
        _terrainShader->Unbind();

        //Save stats
        APP_SETTINGS.renderStats.drawnVertices += verticeCount;
        APP_SETTINGS.renderStats.drawCalls++;
        APP_SETTINGS.renderStats.terrainPasses++;
    }

    void SceneRenderer::AddLightProjection(const glm::mat4 &lightProj)
    {
        _lightProj = lightProj;
    }

    void SceneRenderer::AddCubemap(const std::array<const char*, 6>& faces, const std::string& shader)
    {
        _cubemap = MakeScope<Cubemap>(faces, ResourceManager::GetShader(shader));
    }

    void SceneRenderer::AddTerrain
    (
        const uint32 x, const uint32 z, const float tileSize, const glm::vec3& position, Texture* depthTexture,
        const std::string& texture, const std::string& colormap, const std::string& heightmap
    )
    {
        //Create terrain mesh
        Mesh terrainMesh;
        MeshCreator::CreatePlane(x, z, tileSize, &terrainMesh, ResourceManager::GetHeightmap(heightmap));

        //Create and store model
        _terrainModel = new Model(&terrainMesh);

        //Reposition model
        _terrainModel->ChangePosition(position);

        //Add textures
        _terrainModel->AddTexture(ResourceManager::GetTexture(texture));
        _terrainModel->AddTexture(ResourceManager::GetTexture(colormap));
        _terrainModel->AddTexture(depthTexture);
    }

    void SceneRenderer::AddObject
    (
        const float size, const glm::vec3& rotation, const glm::vec3& position,
        Texture* depthTexture, const std::string& objName, const std::string& objFilepath
    )
    {
        //Get meshes out of obj-File
        std::vector<Mesh> meshes;
        MeshCreator::CreateFromObj(objName, objFilepath, &meshes);

        //Create models out of meshes and store these
        for(const auto& mesh : meshes)
        {
            auto objModel = new Model(&mesh);

            objModel->SetTextureInSlot(depthTexture, 1);
            objModel->ChangeSize(size);
            objModel->ChangeRotation(rotation);
            objModel->ChangePosition(position);

            _modelStorage.push_back(objModel);
        }
    }

    void SceneRenderer::AddWater
    (
        const uint32 x, const uint32 z, const float tileSize, const glm::vec3& position,
        const float waveSpeed, const std::string& dudvMap, const std::string& normalMap,
        Texture* reflectTex, Texture* refractTex, Texture* refractDepthTex
    )
    {
        //Create plane mesh
        Mesh planeMesh;
        MeshCreator::CreatePlane(x, z, tileSize, &planeMesh);

        //Create and store model
        _waterModel = new Model(&planeMesh);

        //Reposition model
        _waterModel->ChangePosition(position);

        //Add textures (ORDER IMPORTANT!)
        _waterModel->AddTexture(reflectTex);
        _waterModel->AddTexture(refractTex);
        _waterModel->AddTexture(ResourceManager::GetTexture(dudvMap));
        _waterModel->AddTexture(ResourceManager::GetTexture(normalMap));
        _waterModel->AddTexture(refractDepthTex);

        //Store other variable(s)
        _waveSpeed = waveSpeed;
    }

    void SceneRenderer::SetTerrainShader(const std::string& terrainShader)
    {
        _terrainShader = ResourceManager::GetShader(terrainShader);
    }

    void SceneRenderer::SetModelShader(const std::string& modelShader)
    {
        _modelShader   = ResourceManager::GetShader(modelShader);
    }

    void SceneRenderer::SetWaterShader(const std::string& waterShader)
    {
        _waterShader   = ResourceManager::GetShader(waterShader);
    }

    Shader* SceneRenderer::GetTerrainShader()
    {
        return _terrainShader;
    }

    Shader* SceneRenderer::GetModelShader()
    {
        return _modelShader;
    }

    Shader* SceneRenderer::GetWaterShader()
    {
        return _waterShader;
    }

    glm::mat4 SceneRenderer::GetProjMatrix()
    {
        return _perspProj;
    }
}