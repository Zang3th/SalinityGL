#pragma once

#include "glm.hpp"
#include "CoreDefines.hpp"

namespace Engine
{
    class Particle
    {
        private:
            glm::vec3 _position,      _velocity;
            float     _gravityFactor, _lifeLength, _rotation, _size, _elapsedTime;

        public:
            Particle
            (
                glm::vec3 position,
                glm::vec3 velocity,
                float     gravity,
                float     lifeLength,
                float     rotation,
                float     size
            );
            bool Update(float dt);

            glm::vec3 GetPosition();
            float     GetRotation();
            float     GetSize();
    };
}