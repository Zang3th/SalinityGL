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
    GameObject* _player = nullptr;
    glm::vec2 _playerSize;
    float _playerVelocity;
	
    Game(unsigned int width, unsigned int height)
        : _state(GAME_ACTIVE), _keys(), _width(width), _height(height), _playerSize(100.0f, 20.0f), _playerVelocity(500.0f)
    {
    	
    }

    ~Game()
    {
        delete _spriteRenderer;
        delete _gameLevelCreator;
        delete _player;
    }

    void init()
    {
    	//create SpriteRenderer
        _spriteRenderer = new SpriteRenderer("res/shader/breakout_vs.glsl", "res/shader/breakout_fs.glsl", _width, _height);
            	
        //create GameLevelCreator
        _gameLevelCreator = new GameLevelCreator(_spriteRenderer, _width, _height);
    	
    	//load level from file
        _gameLevelCreator->generateLevel();
    	
        //player paddle creation 	
        ResourceManager::LoadTexture("res/textures/Paddle.png", "Paddle");
        _player = new GameObject(glm::vec2(_width / 2.0f - _playerSize.x / 2.0f, _height - _playerSize.y), _playerSize, glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, ResourceManager::GetTexture("Paddle"), _spriteRenderer);
    }

    void update(float dt)
    {

    }

    void processInput(float dt)
    {
        if (this->_state == GAME_ACTIVE)
        {
            float velocity = _playerVelocity * dt;
        	
            //move player paddle
            if (this->_keys[GLFW_KEY_A])
            {
                if (_player->_position.x >= 0.0f)
                    _player->_position.x -= velocity;
            }
            if (this->_keys[GLFW_KEY_D])
            {
                if (_player->_position.x <= _width - _player->_size.x)
                    _player->_position.x += velocity;
            }
        }
    }

    void render()
    {
    	//render level
        _gameLevelCreator->renderLevel();

    	//render player
        _player->Draw();
    }
};