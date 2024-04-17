#include "SceneRenderer.hpp"

namespace Engine
{
    // ----- Private -----

    SceneRenderer::SceneRenderer()
        : _moveFactor(0.0f), _waveSpeed(0.0f), _perspProj(glm::perspective(glm::radians(45.0f), Window::GetAspectRatio(), RenderParams::nearPlane, RenderParams::farPlane)),
          _lightProj(0.0f), _terrainModel(nullptr), _waterModel(nullptr), _planeModel(nullptr), _terrainShader(nullptr), _modelShader(nullptr), _waterShader(nullptr)
    {
        Logger::Info("Created", "Renderer", __func__);
    }

    SceneRenderer::~SceneRenderer()
    {
        for(auto const& model : _modelStorage)
            delete model;

        delete _terrainModel;
        delete _waterModel;
        delete _planeModel;
    }

    void SceneRenderer::FlushModel(Model* model, Shader* shader)
    {
        //Get textures and bind them
        uint8_t slot = 0;
        for(const auto& tex : *model->GetTextures())
        {
            if(tex != nullptr)
                tex->BindToSlot(slot);

            slot++;
        }

        //Bind buffers and get vertice count
        model->BindBuffers();
        uint32 verticeCount = model->GetVerticeCount();

        //Set uniforms
        shader->SetUniformMat4f("view", Camera3D::GetViewMatrix());
        shader->SetUniformMat4f("model", model->GetModelMatrix());
        shader->SetUniformMat4f("projection", _perspProj);
        shader->SetUniformVec3f("viewPos", Camera3D::GetPosition());
        shader->SetUniformVec3f("lightPos", LightParams::position);
        shader->SetUniformVec3f("lightColor", LightParams::color);
        shader->SetUniformMat4f("lightProjection", _lightProj);
        shader->SetUniform1i("diffuseTexture", 0);
        shader->SetUniform1i("shadowMap", 1);

        //Render model
        GLCall(glDrawElements(GL_TRIANGLES, verticeCount, GL_UNSIGNED_INT, nullptr))

        //Unbind buffers
        model->UnbindBuffers();

        //Unbind textures
        for(const auto& tex : *model->GetTextures())
        {
            tex->Unbind();
        }

        //Save stats
        RenderStatistics::drawnVertices += verticeCount;
        RenderStatistics::drawCalls++;
    }

    void SceneRenderer::FlushWater()
    {
        GLRenderSettings::SetCullFace(GL_FRONT);

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
        _waterShader->SetUniformVec3f("lightPos", LightParams::position);
        _waterShader->SetUniformVec3f("lightColor", LightParams::color);
        _waterShader->SetUniform1i("reflectionTexture", 0);
        _waterShader->SetUniform1i("refractionTexture", 1);
        _waterShader->SetUniform1i("dudvMap", 2);
        _waterShader->SetUniform1i("normalMap", 3);
        _waterShader->SetUniform1i("depthMap", 4);
        _waterShader->SetUniform1f("moveFactor", _moveFactor);
        _waterShader->SetUniform1f("nearPlane", RenderParams::nearPlane);
        _waterShader->SetUniform1f("farPlane", RenderParams::farPlane);

        //Render model
        GLCall(glDrawElements(GL_TRIANGLES, verticeCount, GL_UNSIGNED_INT, nullptr))

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
        RenderStatistics::drawnVertices += verticeCount;
        RenderStatistics::drawCalls++;
        RenderStatistics::waterPasses++;

        GLRenderSettings::SetCullFace(GL_BACK);
    }

    void SceneRenderer::UpdateMoveFactor()
    {
        _moveFactor += _waveSpeed * (float)Window::GetDeltaTime();
        _moveFactor = (float)fmodf(_moveFactor, 1.0f);
    }

    // ----- Public -----

    void SceneRenderer::Flush(Renderer* renderer)
    {
        //Check for Wireframe-Mode
        if(WindowParams::wireframeRendering)
            GLRenderSettings::EnableWireframe();
        else
            GLRenderSettings::DisableWireframe();

        //Modify water move factor
        UpdateMoveFactor();

        if(_terrainModel)
            FlushTerrain();

        if(_planeModel)
            FlushPlane();

        if(!_modelStorage.empty())
            FlushModels(_modelShader);

        if(_waterModel)
            FlushWater();

        if(_cubemap)
            FlushCubemap();
    }

    void SceneRenderer::FlushModels(Shader* shader)
    {
        for(auto& model : _modelStorage)
        {
            shader->Bind();
            shader->SetUniform1i("normalMap", 2);
            shader->SetUniform1i("gotNormalMap", model->GotNormalMap());
            shader->SetUniform1i("gotDiffuseTex", model->GotDiffuseTex());
            FlushModel(model, shader);
            shader->Unbind();
        }

        RenderStatistics::modelPasses++;
    }

    void SceneRenderer::FlushCubemap()
    {
        GLRenderSettings::SetCullFace(GL_FRONT);
        GLRenderSettings::DisableWireframe();
        RenderStatistics::drawnVertices += _cubemap->Draw(_perspProj, Camera3D::GetViewMatrix());
        RenderStatistics::drawCalls++;
        RenderStatistics::cubemapPasses++;
        GLRenderSettings::SetCullFace(GL_BACK);
    }

    void SceneRenderer::FlushTerrain()
    {
        GLRenderSettings::SetCullFace(GL_FRONT);
        _terrainShader->Bind();
        _terrainShader->SetUniform1i("colorMap", 2);
        FlushModel(_terrainModel, _terrainShader);
        _terrainShader->Unbind();
        RenderStatistics::terrainPasses++;
        GLRenderSettings::SetCullFace(GL_BACK);
    }

    void SceneRenderer::FlushPlane()
    {
        GLRenderSettings::SetCullFace(GL_FRONT);
        _modelShader->Bind();
        FlushModel(_planeModel, _modelShader);
        _modelShader->Unbind();
        RenderStatistics::terrainPasses++;
        GLRenderSettings::SetCullFace(GL_BACK);
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
        _terrainModel->AddTexture(depthTexture);
        _terrainModel->AddTexture(ResourceManager::GetTexture(colormap));
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

            objModel->AddTextureToSlot(depthTexture, 1);
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

    void SceneRenderer::AddPlane
    (
        const uint32 x, const uint32 z, const float tileSize, const glm::vec3& position,
        Texture* depthTexture, const std::optional<const std::string>& texture
    )
    {
        //Create terrain mesh
        Mesh planeMesh;
        MeshCreator::CreatePlane(x, z, tileSize, &planeMesh);

        //Create model
        _planeModel = new Model(&planeMesh);

        //Reposition model
        _planeModel->ChangePosition(position);

        //Add texture(s)
        if(texture.has_value())
        {
            _planeModel->AddTexture(ResourceManager::GetTexture(*texture));
            _planeModel->SetDiffuseTexture();
        }

        _planeModel->AddTextureToSlot(depthTexture, 1);
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