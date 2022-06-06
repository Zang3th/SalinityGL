#pragma once

#include "glm.hpp"
#include "CoreDefines.hpp"
#include <cmath>
#include "Camera.hpp"

namespace Engine
{
    class Particle
    {
        private:
            glm::vec3 _initPosition,   _position,       _initVelocity,  _velocity;
            glm::vec2 _textureOffset0, _textureOffset1;
            float     _gravityFactor,  _lifeLength,     _elapsedTime,    _blendFactor,
                      _initRotation,   _rotation,       _initSize,      _size,          _distanceToCamera;
            uint32    _numberOfRows;

                          void      UpdateTextureCoordInfo();
            [[nodiscard]] glm::vec2 CalculateTextureOffset(uint32 index) const;

    public:
            Particle
            (
                glm::vec3 position,
                glm::vec3 velocity,
                float     gravity,
                float     lifeLength,
                float     rotation,
                float     size,
                uint32    numberOfRows
            );
            bool Update(float dt);
            void Respawn();

            [[nodiscard]] glm::vec3 GetPosition()       const;
            [[nodiscard]] glm::vec2 GetTextureOffset0() const;
            [[nodiscard]] glm::vec2 GetTextureOffset1() const;
            [[nodiscard]] float     GetRotation()       const;
            [[nodiscard]] float     GetSize()           const;
            [[nodiscard]] float     GetBlendFactor()    const;
            [[nodiscard]] float     GetDistanceToCam()  const;
    };
}