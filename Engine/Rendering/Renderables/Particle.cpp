#include "Particle.hpp"

namespace Engine
{
    // ----- Private -----

    void Particle::UpdateTextureCoordInfo()
    {
        float  lifeFactor    = _elapsedTime / _lifeLength;
        uint32 stageCount    = _numberOfRows * _numberOfRows;
        float  atlasProgress = lifeFactor * (float)stageCount;
        auto   index0        = (uint32)(floorf(atlasProgress));
        uint32 index1        = index0 < stageCount - 1 ? index0 + 1 : index0;

        _blendFactor    = (float)fmodf(atlasProgress, 1.0f);
        _textureOffset0 = CalculateTextureOffset(index0);
        _textureOffset1 = CalculateTextureOffset(index1);
    }

    glm::vec2 Particle::CalculateTextureOffset(uint32 index) const
    {
        uint32 col = index % _numberOfRows;
        uint32 row = index / _numberOfRows;

        return {(float)col / (float)_numberOfRows, (float)row /(float)_numberOfRows};
    }

    // ----- Public -----

    Particle::Particle
    (
        glm::vec3 position,
        glm::vec3 velocity,
        float     gravity,
        float     lifeLength,
        float     rotation,
        float     size,
        uint32    numberOfRows
    )
        :   _initPosition(position), _position(position),
            _initVelocity(velocity), _velocity(velocity),
            _textureOffset0(glm::vec2(0.0f)), _textureOffset1(glm::vec2(0.0f)),
            _gravityFactor(gravity),
            _lifeLength(lifeLength), _elapsedTime(0.0f), _blendFactor(0.0f),
            _initRotation(rotation), _rotation(rotation),
            _initSize(size), _size(size),
            _distanceToCamera(0.0f),
            _numberOfRows(numberOfRows)
    {}

    bool Particle::Update(float dt)
    {
        _velocity.y  += AppSettings::GRAVITY * _gravityFactor * dt;
        _position    += _velocity * dt;
        _elapsedTime += dt;

        UpdateTextureCoordInfo();

        _distanceToCamera = glm::length(Camera3D::GetPosition() - _position) * 2.0f;

        return _elapsedTime < _lifeLength;
    }

    void Particle::Respawn()
    {
        _position    = _initPosition;
        _velocity    = _initVelocity;
        _rotation    = _initRotation;
        _size        = _initSize;
        _elapsedTime = 0.0f;
    }

    glm::vec3 Particle::GetPosition() const
    {
        return _position;
    }

    glm::vec2 Particle::GetTextureOffset0() const
    {
        return _textureOffset0;
    }

    glm::vec2 Particle::GetTextureOffset1() const
    {
        return _textureOffset1;
    }

    float Particle::GetRotation() const
    {
        return _rotation;
    }

    float Particle::GetSize() const
    {
        return _size;
    }

    float Particle::GetBlendFactor() const
    {
        return _blendFactor;
    }

    float Particle::GetDistanceToCam() const
    {
        return _distanceToCamera;
    }
}