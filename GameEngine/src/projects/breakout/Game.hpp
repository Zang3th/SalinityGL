#pragma once

#include "SpriteRenderer.hpp"
#include "GameLevelCreator.hpp"
#include "BallObject.hpp"

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game
{
public:
	//Game
    GameState _state;
    bool _keys[1024];
    unsigned int _width, _height;
    SpriteRenderer* _spriteRenderer = nullptr;
    GameLevelCreator* _gameLevelCreator = nullptr;
	
	//Player
    GameObject* _player = nullptr;
    glm::vec2 _playerSize;
    float _playerVelocity;
	
	//Ball
    BallObject* _ball = nullptr;
    glm::vec2 _ballVelocity;
    float _ballRadius;
	
    Game(unsigned int width, unsigned int height)
        : _state(GAME_ACTIVE), _keys(), _width(width), _height(height), _playerSize(100.0f, 20.0f), _playerVelocity(500.0f), _ballVelocity(100.0f, -350.0f), _ballRadius(12.5f)
    {
    	
    }

    ~Game()
    {
        delete _spriteRenderer;
        delete _gameLevelCreator;
        delete _player;
        delete _ball;
    }

    void init()
    {
    	//Create SpriteRenderer
        _spriteRenderer = new SpriteRenderer("res/shader/breakout_vs.glsl", "res/shader/breakout_fs.glsl", _width, _height);
            	
        //Create GameLevelCreator
        _gameLevelCreator = new GameLevelCreator(_spriteRenderer, _width, _height);
    	
    	//Load level from file
        _gameLevelCreator->generateLevel("res/levels/basic.level");
    	
        //Player paddle creation 	
        ResourceManager::LoadTexture("res/textures/Paddle.png", "Paddle");
        _player = new GameObject(glm::vec2(_width / 2.0f - _playerSize.x / 2.0f, _height - _playerSize.y), _playerSize, glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, ResourceManager::GetTexture("Paddle"), _spriteRenderer);

        //Ball creation
        ResourceManager::LoadTexture("res/textures/Cannonball.png", "Ball");
        _ball = new BallObject(_player->_position + glm::vec2(_player->_size.x / 2.0f - _ballRadius, -_ballRadius * 2.0f), _ballRadius, _ballVelocity, ResourceManager::GetTexture("Ball"), _spriteRenderer);
    }

    void update(float dt)
    {
        _ball->Move(dt, _width);
    }

    void processInput(float dt)
    {
        if (this->_state == GAME_ACTIVE)
        {
            float velocity = _playerVelocity * dt;
        	
            //Check for input
            if (_keys[GLFW_KEY_A])
            {
                if (_player->_position.x >= 0.0f)
                {
                    _player->_position.x -= velocity;
                    if (_ball->_stuck)
                        _ball->_position.x -= velocity;
                }                    
            }
            if (_keys[GLFW_KEY_D])
            {
                if (_player->_position.x <= _width - _player->_size.x)
                {
                    _player->_position.x += velocity;
                    if (_ball->_stuck)
                        _ball->_position.x += velocity;
                }                    
            }
        	if (_keys[GLFW_KEY_SPACE])
        	{
                _ball->_stuck = false;
        	}
        }
    }

    void render()
    {
    	//Render level
        _gameLevelCreator->renderLevel();

    	//Render player
        _player->Draw();

    	//Render ball
        _ball->Draw();
    }
};