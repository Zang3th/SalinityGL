#include "SceneRenderer.hpp"

namespace Engine
{
    // ----- Private -----

    SceneRenderer::SceneRenderer(const float nearPlane, const float farPlane, const glm::vec3& lightPos, const glm::vec3& lightCol)
        : _nearPlane(nearPlane), _farPlane(farPlane), _lightPos(lightPos), _lightCol(lightCol),
          _perspProj(glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, _nearPlane, _farPlane)),
          _lightProj(0.0f), _terrainModel(nullptr), _terrainShader(nullptr), _modelShader(nullptr), _waterShader(nullptr)
    {
        Logger::Info("Created", __func__);
    }

    SceneRenderer::~SceneRenderer()
    {
        for(auto const& model : _modelStorage)
            delete model;

        delete _terrainModel;
    }

    void SceneRenderer::FlushCubemap()
    {
        if(_cubemap)
        {
            GLRenderSettings::DisableWireframe();
            RENDER_STATS.drawnVertices += _cubemap->Draw(_perspProj, Camera3D::GetViewMatrix());
            RENDER_STATS.drawCalls++;
            RENDER_STATS.cubemapPasses++;
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
        RENDER_STATS.drawnVertices += verticeCount;
        RENDER_STATS.drawCalls++;
        RENDER_STATS.terrainPasses++;
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
        RENDER_STATS.drawnVertices += verticeCount;
        RENDER_STATS.drawCalls++;
    }

    // ----- Public -----

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

    void SceneRenderer::AddLightProjection(const glm::mat4 &lightProj)
    {
        _lightProj = lightProj;
    }

    void SceneRenderer::Flush(Renderer* renderer)
    {
        //Check for Wireframe-Mode
        if(WIREFRAME_RENDERING)
            GLRenderSettings::EnableWireframe();
        else
            GLRenderSettings::DisableWireframe();

        FlushTerrain();
        FlushModels(_modelShader);
        FlushCubemap();
    }

    void SceneRenderer::FlushModels(Shader* shader)
    {
        for(auto& model : _modelStorage)
        {
            FlushModel(model, shader);
        }

        RENDER_STATS.modelPasses++;
    }

    void SceneRenderer::AddCubemap(const std::array<const char*, 6>& faces, const std::string& shader)
    {
        _cubemap = MakeScope<Cubemap>(faces, ResourceManager::GetShader(shader));
    }

    void SceneRenderer::AddTerrain
    (
        const uint32 x, const uint32 z, const float tileSize, const glm::vec3& position, Texture* depthTexture,
        const std::string& heightmapFilepath, const std::string& texture, const std::string& colormap
    )
    {
        //Create terrain mesh
        Mesh terrainMesh;
        Heightmap heightmap(heightmapFilepath);
        MeshCreator::CreatePlane(x, z, tileSize, &terrainMesh, &heightmap);

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
}