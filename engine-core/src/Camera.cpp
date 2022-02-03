#include "Camera.hpp"

namespace Core
{
    // ----- Public -----

    Camera::Camera(glm::vec3 position, float yaw, float pitch, float movementSpeed)
        :   _position(position),
            _front(0.0f),
            _up(0.0f),
            _right(0.0f),
            _worldUp(0.0f, 1.0f, 0.0f),
            _yaw(yaw),
            _pitch(pitch),
            _movementSpeed(movementSpeed),
            _mouseSensitivity(0.1f)
    {
        Update();
    }

    void Camera::Update()
    {
        //Calculate the new front vector
        glm::vec3 front;
        front.x = (float)(cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)));
        front.y = (float)(sin(glm::radians(_pitch)));
        front.z = (float)(sin(glm::radians(_yaw)) * cos(glm::radians(_pitch)));
        _front = glm::normalize(front);

        //Also, re-calculate the right and up vector
        _right = glm::normalize(glm::cross(_front, _worldUp)); //Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement
        _up = glm::normalize(glm::cross(_right, _front));
    }

    void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = _movementSpeed * deltaTime;

        if (direction == FORWARD)
            _position += _front * velocity;

        if (direction == BACKWARD)
            _position -= _front * velocity;

        if (direction == LEFT)
            _position -= _right * velocity;

        if (direction == RIGHT)
            _position += _right * velocity;

        if (direction == UP)
            _position += _up * velocity;

        if (direction == DOWN)
            _position -= _up * velocity;
    }

    void Camera::ProcessMouseMovement(float xOffset, float yOffset)
    {
        xOffset *= _mouseSensitivity;
        yOffset *= _mouseSensitivity;

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
        Update();
    }

    glm::vec3 Camera::GetPosition() const
    {
        return _position;
    }

    glm::vec3 Camera::GetUp() const
    {
        return _up;
    }

    glm::vec3 Camera::GetFront() const
    {
        return _front;
    }

    float Camera::GetYaw() const
    {
        return _yaw;
    }

    float Camera::GetPitch() const
    {
        return _pitch;
    }

    void Camera::SetPosition(const glm::vec3& position)
    {
        _position = position;
    }

    void Camera::InvertPitch()
    {
        _pitch = -_pitch;
    }
}