#pragma once

#include "SpriteRenderer.hpp"
#include "Texture.hpp"
#include "ResourceManager.hpp"

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
    Texture*       _spriteTexture;
    SpriteRenderer _spriteRenderer;
	
    // constructor
    GameObject(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity, glm::vec3 color, float rotation, Texture* spriteTexture, SpriteRenderer& spriteRenderer, bool solid = false, bool destroyed = false)
	    : _position(pos), _size(size), _velocity(velocity), _color(color), _rotation(rotation), _spriteTexture(spriteTexture), _spriteRenderer(spriteRenderer), _solid(solid), _destroyed(destroyed)
    {
	    
    }

	// draw sprite
    virtual void Draw()
    {
        _spriteRenderer.DrawSprite(_spriteTexture, _position, _size, _rotation, _color);
    }
};

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game
{
public:
    GameState _state;
    bool _keys[1024];
    unsigned int _width, _height;
    SpriteRenderer _spriteRenderer;
    Texture* _brickTexture = nullptr;
    GameObject *GO = nullptr, *G1 = nullptr;

	
    Game(unsigned int width, unsigned int height)
        : _state(GAME_ACTIVE), _keys(), _width(width), _height(height)
    {
        _spriteRenderer = SpriteRenderer();        
    }

    ~Game()
    {
        delete _brickTexture;
        delete GO;
        ResourceManager::DeleteTextures();
    }

    void init()
    {
        _spriteRenderer.initRenderData("res/shader/breakout_vs.glsl", "res/shader/breakout_fs.glsl");
    	
        ResourceManager::LoadTexture("res/textures/Brick.jpg", "Brick");
        ResourceManager::LoadTexture("res/textures/Dirt.jpg", "Dirt");
    	
        GO = new GameObject(glm::vec2(200.0f, 200.0f), glm::vec2(200.0f, 200.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, ResourceManager::GetTexture("Brick"), _spriteRenderer);
    	G1 = new GameObject(glm::vec2(600.0f, 600.0f), glm::vec2(200.0f, 200.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, ResourceManager::GetTexture("Dirt"), _spriteRenderer);
     }

    void update(float dt)
    {

    }

    void processInput(float dt)
    {

    }

    void render()
    {
        //Render GameObjects
        GO->Draw();
        G1->Draw();
    }
};