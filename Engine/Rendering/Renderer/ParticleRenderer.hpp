#pragma once

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

#include <algorithm>

namespace Engine
{
    class ParticleRenderer
    {
        private:
            Scope<VertexArray>           _vao;
            Scope<VertexBuffer>          _vboVert, _vboModel, _vboTex, _vboBlend;
            Texture                     *_textureAtlas;
            Shader                      *_shader;
            glm::vec3                    _position;
            uint32                       _count;
            uint32                       _numberOfRows;
            float                        _size;
            float                        _speed;
            float                        _gravityComplient;
            float                        _lifeLength;
            float                        _respawnTreshold;
            uint32                       _verticeCount;
            std::vector<Particle*>       _pointerStorage;
            std::vector<glm::mat4>       _modelViewStorage;
            std::vector<glm::vec4>       _texOffsetStorage;
            std::vector<float>           _blendFactorStorage;

            void      InitGpuStorage();
            void      UpdateGpuStorage();
            void      GenerateParticles();
            glm::mat4 GetModelViewMatrix(Particle* particle);

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