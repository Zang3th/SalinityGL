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
        :   _initPosition(position), _position(position),
            _initVelocity(velocity), _velocity(velocity),
            _gravityFactor(gravity),
            _initLifeLength(lifeLength), _lifeLength(lifeLength),
            _initRotation(rotation), _rotation(rotation),
            _initSize(size), _size(size),
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

    void Particle::Respawn()
    {
        _position    = _initPosition;
        _velocity    = _initVelocity;
        _lifeLength  = _initLifeLength;
        _elapsedTime = 0.0f;
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