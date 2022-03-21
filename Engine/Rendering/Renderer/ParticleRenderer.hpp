#pragma once

#include "CoreDefines.hpp"
#include "Particle.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Window.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "ErrorManager.hpp"
#include "Random.hpp"
#include <algorithm>

namespace Engine
{
    class ParticleRenderer
    {
        private:
            struct ParticleBinding
            {
                Particle* particle;
                glm::mat4 modelView;

                ParticleBinding(Particle* particle, glm::mat4 modelView)
                    : particle(particle), modelView(modelView)
                {};
            };

            Ref<VertexArray>             _vao;
            Texture                     *_textureAtlas;
            Shader                      *_shader;
            glm::vec3                    _position;
            uint32                       _count;
            float                        _size;
            float                        _speed;
            float                        _gravityComplient;
            float                        _lifeLength;
            float                        _respawnTreshold;
            uint32                       _verticeCount;
            std::vector<ParticleBinding> _particleStorage;

            Ref<VertexArray> CreateParticleVao();
            void             GenerateParticles();
            glm::mat4        GetModelViewMatrix(Particle* particle);

        public:
            ParticleRenderer
            (
                Texture*  textureAtlas,
                Shader*   shader,
                glm::vec3 position,
                uint32    count,
                float     size,
                float     speed,
                float     gravityComplient,
                float     lifeLength,
                float     respawnThreshold
            );
            ~ParticleRenderer();
            [[nodiscard]] uint32 Render(const glm::mat4& projMatrix);
    };
}