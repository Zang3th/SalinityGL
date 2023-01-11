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

            glm::vec3                    _position;
            uint32                       _count;
            uint32                       _numberOfRows;
            float                        _size;
            float                        _speed;
            float                        _gravityComplient;
            float                        _lifeLength;
            float                        _respawnTreshold;
            uint32                       _verticeCount;
            Texture                     *_textureAtlas;
            Shader                      *_shader;

            std::vector<Particle*>       _pointerStorage;
            std::vector<glm::mat4>       _modelViewStorage;
            std::vector<glm::vec4>       _texOffsetStorage;
            std::vector<float>           _blendFactorStorage;

            ParticleRenderer
            (
                glm::vec3 position, uint32 count, float size, float speed, float gravityComplient,
                float lifeLength, float respawnThreshold, Texture* textureAtlas, Shader* shader
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