#pragma once

#include "SpriteRenderer.hpp"
#include "Texture.hpp"

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
	
    Game(unsigned int width, unsigned int height)
        : _state(GAME_ACTIVE), _keys(), _width(width), _height(height)
    {
        _spriteRenderer = SpriteRenderer();
        
    }

    ~Game()
    {

    }

    void init()
    {
        _spriteRenderer.initRenderData("res/shader/breakout_vs.glsl", "res/shader/breakout_fs.glsl");
        _brickTexture = new Texture("res/textures/Brick.jpg", 0);
    }

    void update(float dt)
    {

    }

    void processInput(float dt)
    {

    }

    void render()
    {
        _spriteRenderer.DrawSprite(_brickTexture, glm::vec2(200.0f, 200.0f), glm::vec2(200.0f, 200.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    }
};