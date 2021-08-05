#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Core
{
    class Camera
    {
        private:
            glm::vec3 _position;
            glm::vec3 _front;
            glm::vec3 _up;
            glm::vec3 _right;
            glm::vec3 _worldUp;
            float _yaw;
            float _pitch;
            float _movementSpeed;
            float _mouseSensitivity;

            void UpdateCameraVectors();

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

            Camera(glm::vec3 position, float yaw, float pitch, float movementSpeed);
            inline glm::mat4 GetViewMatrix(){return glm::lookAt(_position, _position + _front, _up);}
            void ProcessKeyboard(Camera_Movement direction, float deltaTime);
            void ProcessMouseMovement(float xOffset, float yOffset);
            glm::vec3 GetPosition() const;
            glm::vec3 GetFront() const;
            float GetYaw() const;
            float GetPitch() const;
    };
}