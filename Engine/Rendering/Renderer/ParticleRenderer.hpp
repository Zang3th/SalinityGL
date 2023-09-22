#pragma once

#include "Renderer.hpp"
#include "Types.hpp"
#include "Particle.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Camera3D.hpp"
#include "Window.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "ErrorManager.hpp"
#include "Random.hpp"
#include "SceneRenderer.hpp"

#include <algorithm>

namespace Engine
{
    class ParticleRenderer final : public Renderer
    {
        friend class RenderManager;

        private:
            Scope<VertexArray>           _vao;
            Scope<VertexBuffer>          _vboVert, _vboModel, _vboTex, _vboBlend;

            uint32                       _count, _numberOfRows, _verticeCount;
            float                        _size, _speed, _gravityCompliance, _lifeLength, _respawnTreshold;
            Texture                     *_textureAtlas;
            Shader                      *_shader;
            glm::vec3                    _position;

            std::vector<Particle*>       _particleStorage;
            std::vector<glm::mat4>       _modelViewStorage;
            std::vector<glm::vec4>       _texOffsetStorage;
            std::vector<float>           _blendFactorStorage;

            ParticleRenderer
            (
                uint32 count, float size, float speed, float gravityCompliance, float lifeLength,
                float respawnThreshold, Texture* textureAtlas, Shader* shader, const glm::vec3& position
            );
            ~ParticleRenderer() final;

            void      InitGpuStorage();
            void      UpdateGpuStorage();
            void      GenerateParticles();
            glm::mat4 GetModelViewMatrix(Particle* particle);

        public:
            void Flush(Renderer* sceneRenderer) final;
    };
}