#include "Camera.hpp"

namespace Core
{
    // ----- Private -----

    void Camera::UpdateCameraVectors()
    {
        //Calculate the new front vector
        glm::vec3 front;
        front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        front.y = sin(glm::radians(_pitch));
        front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        _front = glm::normalize(front);

        //Also, re-calculate the right and up vector
        _right = glm::normalize(glm::cross(_front, _worldUp)); //Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement
        _up = glm::normalize(glm::cross(_right, _front));
    }

    // ----- Public -----

    Camera::Camera(glm::vec3 position, float yaw, float pitch, float movementSpeed)
    : _position(position), _front(0.0f), _up(0.0f), _right(0.0f), _worldUp(0.0f, 1.0f, 0.0f), _yaw(yaw), _pitch(pitch), _movementSpeed(movementSpeed), _mouseSensitivity(0.1f)
    {
        UpdateCameraVectors();
    }

    void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = _movementSpeed * deltaTime;

        if (direction == FORWARD)
            Position += _front * velocity;

        if (direction == BACKWARD)
            Position -= _front * velocity;

        if (direction == LEFT)
            Position -= _right * velocity;

        if (direction == RIGHT)
            Position += _right * velocity;

        if (direction == UP)
            Position += _up * velocity;

        if (direction == DOWN)
            Position -= _up * velocity;
    }

    void Camera::ProcessMouseMovement(float xOffset, float yOffset)
    {
        xOffset *= MouseSensitivity;
        yOffset *= MouseSensitivity;

        _yaw += xOffset;
        _pitch += yOffset;

        //Make sure that when pitch is out of bounds, screen doesn't get flipped
        if(_pitch > 89.9f)
            _pitch = 89.9f;

        if(_pitch < -89.9f)
            _pitch = -89.9f;

        if(_yaw > 359.9f)
            _yaw = 0.0f;

        if(_yaw < -359.9f)
            _yaw = 0.0f;

        //Update front, right and up vectors using the updated Euler angles
        UpdateCameraVectors();
    }

    glm::vec3 Camera::GetPosition() const
    {
        return _position;
    }

    float Camera::GetYaw() const
    {
        return _yaw;
    }

    float Camera::GetPitch() const
    {
        return _pitch;
    }
}