#pragma once

#include "CoreDefines.hpp"
#include "Particle.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Window.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "ErrorManager.hpp"

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
            Shader                      *_shader;
            glm::vec3                    _position;
            uint32                       _count;
            float                        _speed;
            float                        _gravityComplient;
            float                        _lifeLength;
            uint32                       _verticeCount;
            std::vector<ParticleBinding> _particleStorage;

            Ref<VertexArray> CreateParticleVao();
            void             GenerateParticles();
            glm::mat4        GetModelViewMatrix(Particle* particle);

        public:
            ParticleRenderer
            (
                Shader*   shader,
                glm::vec3 position,
                uint32    count,
                float     speed,
                float     gravityComplient,
                float     lifeLength
            );
            ~ParticleRenderer();
            [[nodiscard]] uint32 Render(const glm::mat4& projMatrix);
    };
}