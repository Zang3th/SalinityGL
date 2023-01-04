#include "SceneRenderer.hpp"

namespace Engine
{
    // ----- Private -----

    SceneRenderer::SceneRenderer(const float nearPlane, const float farPlane, const glm::vec3 lightPos, const glm::vec3 lightCol)
        : _nearPlane(nearPlane), _farPlane(farPlane), _lightPos(lightPos), _lightCol(lightCol),
          _perspProj(glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, _nearPlane, _farPlane)),
          _terrainModel(nullptr), _terrainShader(nullptr)
    {

    }

    SceneRenderer::~SceneRenderer()
    {
        for(auto const& model : _modelStorage)
            delete model;
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

    void SceneRenderer::FlushTerrain(Renderer* shadowRenderer)
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
        _terrainShader->SetUniformMat4f("lightProjection", ((ShadowRenderer*)shadowRenderer)->GetLightProjection());
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

    // ----- Public -----

    void SceneRenderer::Flush(Renderer* shadowRenderer)
    {
        //Check for Wireframe-Mode
        if(WIREFRAME_RENDERING)
            GLRenderSettings::EnableWireframe();
        else
            GLRenderSettings::DisableWireframe();

        FlushCubemap();
        FlushTerrain(shadowRenderer);
    }

    void SceneRenderer::FlushModels(Shader* shader)
    {

    }

    void SceneRenderer::AddCubemap(const std::array<const char*, 6>& faces, const std::string& shader)
    {
        _cubemap = MakeScope<Cubemap>(faces, ResourceManager::GetShader(shader));
    }

    void SceneRenderer::AddTerrain
    (
            uint32             x,
            uint32             z,
            float              tileSize,
            glm::vec3          position,
            Texture*           depthTexture,
            const std::string& heightmapFilepath,
            const std::string& texture,
            const std::string& colormap,
            const std::string& shader
    )
    {
        //Create terrain mesh
        Mesh terrainMesh;
        Heightmap heightmap(heightmapFilepath);
        MeshCreator::CreatePlane(x, z, tileSize, &terrainMesh, &heightmap);

        //Create and store model
        _terrainModel = new Model(&terrainMesh);
        _modelStorage.push_back(_terrainModel);

        //Reposition model
        _terrainModel->ChangePosition(position);

        //Add textures
        _terrainModel->AddTexture(ResourceManager::GetTexture(texture));
        _terrainModel->AddTexture(ResourceManager::GetTexture(colormap));
        _terrainModel->AddTexture(depthTexture);

        //Save shader
        _terrainShader = ResourceManager::GetShader(shader);
    }
}