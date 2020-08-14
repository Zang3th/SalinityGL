#pragma once

#include "SpriteRenderer.hpp"
#include "ResourceManager.hpp"
#include "vector"

class GameObject
{
public:
    // object state
    glm::vec2   _position, _size, _velocity;
    glm::vec3   _color;
    float       _rotation;
    bool        _solid;
    bool        _destroyed;

    // render state
    Texture* _spriteTexture;
    SpriteRenderer* _spriteRenderer;

    // constructor
    GameObject(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity, glm::vec3 color, float rotation, Texture* spriteTexture, SpriteRenderer* spriteRenderer, bool solid = false, bool destroyed = false)
        : _position(pos), _size(size), _velocity(velocity), _color(color), _rotation(rotation), _spriteTexture(spriteTexture), _spriteRenderer(spriteRenderer), _solid(solid), _destroyed(destroyed)
    {

    }

    // draw sprite
    virtual void Draw()
    {
        _spriteRenderer->DrawSprite(_spriteTexture, _position, _size, _rotation, _color);
    }
};

class GameLevelCreator
{
private:
    SpriteRenderer* _spriteRenderer = nullptr;
    unsigned int _width, _height;
    std::vector<GameObject> _bricks;

	void init()
	{
        ResourceManager::LoadTexture("res/textures/Background_1.jpg", "Background");
        ResourceManager::LoadTexture("res/textures/Block.jpg", "Block");
        ResourceManager::LoadTexture("res/textures/Block_solid.jpg", "Block_solid");		
	}
	
public:
	GameLevelCreator(SpriteRenderer* spriteRenderer, unsigned int width, unsigned int height)
		: _spriteRenderer(spriteRenderer), _width(width), _height(height)
	{
        this->init();
	}

    ~GameLevelCreator()
	{
        ResourceManager::DeleteTextures();
	}
	
    void loadFromFile(const std::string& filepath)
    {
        _bricks.emplace_back(glm::vec2(0.0f, 0.0f), glm::vec2(this->_width, this->_height), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, ResourceManager::GetTexture("Background"), _spriteRenderer);
        _bricks.emplace_back(glm::vec2(200.0f, 200.0f), glm::vec2(200.0f, 200.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, ResourceManager::GetTexture("Block"), _spriteRenderer);
        _bricks.emplace_back(glm::vec2(600.0f, 600.0f), glm::vec2(200.0f, 200.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, ResourceManager::GetTexture("Block_solid"), _spriteRenderer);
    }

	void renderLevel()
	{
		for(auto bricks: _bricks)
		{
            bricks.Draw();
		}
	}
};