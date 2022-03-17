#include "Particle.hpp"

namespace Engine
{
    // ----- Public -----

    Particle::Particle
    (
        glm::vec3 position,
        glm::vec3 velocity,
        float     gravity,
        float     lifeLength,
        float     rotation,
        float     size
    )
        :   _position(position),
            _velocity(velocity),
            _gravityFactor(gravity),
            _lifeLength(lifeLength),
            _rotation(rotation),
            _size(size),
            _elapsedTime(0.0f)
    {

    }

    bool Particle::Update(float dt)
    {
        _velocity.y  += GRAVITY * _gravityFactor * dt;
        _position    += _velocity * dt;
        _elapsedTime += dt;

        return _elapsedTime < _lifeLength;
    }

    glm::vec3 Particle::GetPosition()
    {
        return _position;
    }

    float Particle::GetRotation()
    {
        return _rotation;
    }

    float Particle::GetSize()
    {
        return _size;
    }
}