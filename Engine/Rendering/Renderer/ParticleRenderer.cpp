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
            //TODO: Slightly randomize parameters to deviate the particles a bit

            //Create parameters
            glm::vec3 pos  = _position;
            glm::vec3 vel  = glm::vec3(0.75f, 0.75f, 0.75f) * _speed;
            float     grav = _gravityComplient;
            float     life = _lifeLength;
            float     rot  = 0.0f;
            float     size = 1.0f;

            //Create particle
            Particle* particle = new Particle(pos, vel, grav, life, rot, size);

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

        //Apply transformations
        model = glm::translate(model, particle->GetPosition());
        model = glm::rotate(model, glm::radians(particle->GetRotation()), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(particle->GetSize()));

        return view * model;
    }

    // ----- Public -----

    ParticleRenderer::ParticleRenderer
    (
        Shader*   shader,
        glm::vec3 position,
        uint32    count,
        float     speed,
        float     gravityComplient,
        float     lifeLength
    )
        :   _vao(CreateParticleVao()),
            _shader(shader),
            _position(position),
            _count(count),
            _speed(speed),
            _gravityComplient(gravityComplient),
            _lifeLength(lifeLength),
            _verticeCount(6)
    {
        GenerateParticles();
    }

    ParticleRenderer::~ParticleRenderer()
    {
        for(auto const& p : _particleStorage)
            delete p.particle;
    }

    //TODO: Optimize Rendering via better Resourcebinding and/or Instanced Rendering
    uint32 ParticleRenderer::Render(const glm::mat4& projMatrix)
    {
        //Iterate over all particles
        for(uint32 i = 0; i < _count; i++)
        {
            //Get current particle
            ParticleBinding particleBinding = _particleStorage[i];

            //Update particle
            //particleBinding.particle->Update((float)Window::GetDeltaTime());
            particleBinding.modelView = GetModelViewMatrix(particleBinding.particle);

            //Bind shader
            _shader->Bind();

            //Set uniforms
            _shader->SetUniformVec4f("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            _shader->SetUniformMat4f("modelView", particleBinding.modelView);
            _shader->SetUniformMat4f("projection", projMatrix);

            //Bind vao
            _vao->Bind();

            //Render particle
            GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));

            //Unbind vao
            _vao->Unbind();

            //Unbind shader
            _shader->Unbind();
        }

        //Return rendered particle count
        return _count;
    }
}