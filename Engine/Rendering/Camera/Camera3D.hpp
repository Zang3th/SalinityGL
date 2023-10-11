#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "GlobalParams.hpp"

namespace Engine
{
    class Camera3D
    {
        private:
            inline static glm::vec3 _position         = glm::vec3(0.0f, 0.0f, 0.0f);
            inline static glm::vec3 _front            = glm::vec3(0.0f, 0.0f, 0.0f);
            inline static glm::vec3 _up               = glm::vec3(0.0f, 0.0f, 0.0f);
            inline static glm::vec3 _right            = glm::vec3(0.0f, 0.0f, 0.0f);
            inline static glm::vec3 _worldUp          = glm::vec3(0.0f, 1.0f, 0.0f);
            inline static float     _yaw              = 0.0f;
            inline static float     _pitch            = 0.0f;
            inline static float     _movementSpeed    = 0.0f;
            inline static float     _mouseSensitivity = 0.1f;

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

            Camera3D() = delete;
            static void Init();
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
            static void ResetPosition();
            static void InvertPitch();
    };
}