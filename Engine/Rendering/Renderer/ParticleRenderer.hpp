#pragma once

#include "Renderer.hpp"
#include "Types.hpp"
#include "Particle.hpp"
#include "GLTexture.hpp"
#include "Shader.hpp"
#include "Camera3D.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Random.hpp"
#include "SceneRenderer.hpp"

#include <algorithm>

namespace Engine
{
    class ParticleRenderer final : public Renderer
    {
        private:
            Scope<VertexArray>      _vao;
            Scope<VertexBuffer>     _vboVert, _vboModel, _vboTex, _vboBlend;

            GLTexture*              _glTextureAtlas;
            Shader*                 _shader;
            uint32                  _count, _numberOfRows, _verticeCount;
            float                   _size, _speed, _gravityCompliance, _lifeLength, _respawnTreshold;
            glm::vec3               _position;

            std::vector<Particle*>  _particleStorage;
            std::vector<glm::mat4>  _modelViewStorage;
            std::vector<glm::vec4>  _texOffsetStorage;
            std::vector<float>      _blendFactorStorage;

            void      InitGpuStorage();
            void      UpdateGpuStorage();
            void      GenerateParticles();
            glm::mat4 GetModelViewMatrix(const Particle* particle);

        public:
            ParticleRenderer
            (
                uint32 count, float size, float speed, float gravityCompliance, float lifeLength,
                float respawnThreshold, const std::string& glTextureAtlas, const std::string& shader, const glm::vec3& position
            );
            ~ParticleRenderer() override;

            void Flush(Renderer* sceneRenderer) override;
    };
}
