#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

namespace Core
{
    class Camera
    {
        private:
            inline static glm::vec3   _position         = glm::vec3();
            inline static glm::vec3   _front            = glm::vec3();
            inline static glm::vec3   _up               = glm::vec3();
            inline static glm::vec3   _right            = glm::vec3();
            inline static glm::vec3   _worldUp          = glm::vec3();
            inline static float       _yaw              = float();
            inline static float       _pitch            = float();
            inline static float       _movementSpeed    = float();
            inline static float       _mouseSensitivity = float();

        public:
            enum Camera_Movement
            {
                FORWARD,
                BACKWARD,
                LEFT,
                RIGHT,
                UP,
                DOWN
            };

            Camera() = delete;
            static void Init(glm::vec3 position, float yaw, float pitch, float movementSpeed);
            static void Update();
            static inline glm::mat4 GetViewMatrix(){return glm::lookAt(_position, _position + _front, _up);}
            static void ProcessKeyboard(Camera_Movement direction, float deltaTime);
            static void ProcessMouseMovement(float xOffset, float yOffset);
            [[nodiscard]] static glm::vec3 GetPosition();
            [[nodiscard]] static glm::vec3 GetFront();
            [[nodiscard]] static glm::vec3 GetUp();
            [[nodiscard]] static float GetYaw();
            [[nodiscard]] static float GetPitch();
            static void SetPosition(const glm::vec3& position);
            static void InvertPitch();
    };
}