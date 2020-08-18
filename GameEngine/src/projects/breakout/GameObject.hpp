#pragma once

#include "SpriteRenderer.hpp"
#include "Texture.hpp"

class GameObject
{
public:
    //Object state
    glm::vec2   _position, _size, _velocity;
    glm::vec3   _color;
    float       _rotation;
    bool        _solid;
    bool        _destroyed;

    //Render state
    Texture* _spriteTexture = nullptr;
    SpriteRenderer* _spriteRenderer = nullptr;

    //Constructor
    GameObject(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity, glm::vec3 color, float rotation, Texture* spriteTexture, SpriteRenderer* spriteRenderer, bool solid = false, bool destroyed = false)
        : _position(pos), _size(size), _velocity(velocity), _color(color), _rotation(rotation), _spriteTexture(spriteTexture), _spriteRenderer(spriteRenderer), _solid(solid), _destroyed(destroyed)
    {

    }

    //Draw sprite
    virtual void Draw()
    {
        _spriteRenderer->DrawSprite(_spriteTexture, _position, _size, _rotation, _color);
    }
};