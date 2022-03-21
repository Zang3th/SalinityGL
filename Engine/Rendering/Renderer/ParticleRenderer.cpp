#include "ParticleRenderer.hpp"

namespace Engine
{
    // ----- Private -----

    Ref<VertexArray> ParticleRenderer::CreateParticleVao()
    {
        //Create and bind vao
        Ref<VertexArray> vao = MakeRef<VertexArray>();
        vao->Bind();

        //Create data
        static const float vertices[] =
        {
            -0.5f, 0.5f,
            -0.5f, -0.5f,
            0.5f, 0.5f,
            0.5f, -0.5f
        };

        //Create vbo, send it data and configure vao
        VertexBuffer vbo(vertices, sizeof(vertices));
        vao->DefineAttributes(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

        //Unbind vao
        vao->Unbind();

        return vao;
    }

    void ParticleRenderer::GenerateParticles()
    {
        //Iterate over particle count
        for(uint32 i = 0; i < _count; i++)
        {
            //Create and randomize the parameters a bit to deviate the particles
            glm::vec3 pos  = _position;
            glm::vec3 vel  = glm::vec3
                            (2.0f  * ((Random::GetFloat() - 0.5f) * 2.0f),
                             25.0f,
                             2.0f  * ((Random::GetFloat() - 0.5f) * 2.0f)) * _speed;
            float     grav = _gravityComplient;
            float     life = _lifeLength + ((Random::GetFloat() - 0.5f) * 2.0f);
            float     rot  = 0.0f;
            float     size = _size * Random::GetFloat();

            //Create particle
            Particle* particle = new Particle(pos, vel, grav, life, rot, size, _textureAtlas->GetNumberOfRows());

            //Get initial modelViewMatrix
            glm::mat4 modelView = GetModelViewMatrix(particle);

            //Save particle binding in particle storage vector
            _particleStorage.emplace_back(ParticleBinding(particle, modelView));
        }
    }

    glm::mat4 ParticleRenderer::GetModelViewMatrix(Particle* particle)
    {
        //Create model matrix
        glm::mat4 model(1.0f);

        //Get view matrix
        glm::mat4 view = Camera::GetViewMatrix();

        //Set upper 3x3 part of the model matrix to the transpose of the view matrix to negate any rotation. Then apply transformations
        model = glm::translate(model, particle->GetPosition());
        model[0][0] = view[0][0];
        model[0][1] = view[1][0];
        model[0][2] = view[2][0];
        model[1][0] = view[0][1];
        model[1][1] = view[1][1];
        model[1][2] = view[2][1];
        model[2][0] = view[0][2];
        model[2][1] = view[1][2];
        model[2][2] = view[2][2];
        model = glm::rotate(model, glm::radians(particle->GetRotation()), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(particle->GetSize()));

        return view * model;
    }

    // ----- Public -----

    ParticleRenderer::ParticleRenderer
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
    )
        :   _vao(CreateParticleVao()),
            _textureAtlas(textureAtlas),
            _shader(shader),
            _position(position),
            _count(count),
            _size(size),
            _speed(speed),
            _gravityComplient(gravityComplient),
            _lifeLength(lifeLength),
            _respawnTreshold(respawnThreshold),
            _verticeCount(4)
    {
        GenerateParticles();
    }

    ParticleRenderer::~ParticleRenderer()
    {
        for(auto const& p : _particleStorage)
            delete p.particle;
    }

    //TODO: Optimize Rendering via Instanced Rendering
    uint32 ParticleRenderer::Render(const glm::mat4& projMatrix)
    {
        //Bind shader
        _shader->Bind();

        //Bind texture
        _textureAtlas->Bind();

        //Bind vao
        _vao->Bind();

        //Iterate over all particles
        for(uint32 i = 0; i < _count; i++)
        {
            //Get current particle
            ParticleBinding particleBinding = _particleStorage[i];
            Particle* particle = particleBinding.particle;

            if(particle->Update((float)Window::GetDeltaTime()))
            {
                if(particle->GetPosition().y < _respawnTreshold)
                {
                    particleBinding.modelView = GetModelViewMatrix(particle);

                    //Create vec2 with texture coordinate information
                    glm::vec2 texCoordInfo(particle->GetNumberOfRows(), particle->GetBlendFactor());

                    //Set uniforms
                    _shader->SetUniformMat4f("modelView", particleBinding.modelView);
                    _shader->SetUniformMat4f("projection", projMatrix);
                    _shader->SetUniformVec2f("texOffset0", particle->GetTextureOffset0());
                    _shader->SetUniformVec2f("texOffset1", particle->GetTextureOffset1());
                    _shader->SetUniformVec2f("texCoordInfo", texCoordInfo);

                    //Render particle
                    GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, _verticeCount));
                }
                else
                {
                    particle->Respawn();
                }
            }
            else
            {
                particle->Respawn();
            }
        }

        //Sort particles depending on the distance to the camera
        std::sort(_particleStorage.begin(), _particleStorage.end(),
                  [] (ParticleBinding const& a, ParticleBinding const& b)
                  { return a.particle->GetDistanceToCam() < b.particle->GetDistanceToCam(); });

        //Unbind vao
        _vao->Unbind();

        //Unbind shader
        _shader->Unbind();

        //Return rendered particle count
        return _count;
    }
}