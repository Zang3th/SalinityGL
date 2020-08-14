#pragma once

#include "SpriteRenderer.hpp"
#include "GameLevelCreator.hpp"

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
    SpriteRenderer* _spriteRenderer = nullptr;
    GameLevelCreator* _gameLevelCreator = nullptr;

	
    Game(unsigned int width, unsigned int height)
        : _state(GAME_ACTIVE), _keys(), _width(width), _height(height)
    {
    	
    }

    ~Game()
    {
        delete _spriteRenderer;
        delete _gameLevelCreator;
    }

    void init()
    {
    	//create SpriteRenderer
        _spriteRenderer = new SpriteRenderer("res/shader/breakout_vs.glsl", "res/shader/breakout_fs.glsl");
            	
        //create GameLevelCreator
        _gameLevelCreator = new GameLevelCreator(_spriteRenderer, _width, _height);
    	
    	//load level from file
        _gameLevelCreator->loadFromFile("res/level/first.level");
    }

    void update(float dt)
    {

    }

    void processInput(float dt)
    {

    }

    void render()
    {
    	//render level
        _gameLevelCreator->renderLevel();
    }
};