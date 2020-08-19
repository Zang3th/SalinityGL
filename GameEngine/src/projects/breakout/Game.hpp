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
private:
    bool CollisionOccured(GameObject& GO1, GameObject& GO2)
    {
	    //Check collision on x-axis
        bool collisionX_0 = GO1._position.x + GO1._size.x >= GO2._position.x; //Right side of GO1 greater than left side of GO2
        bool collisionX_1 = GO2._position.x + GO2._size.x >= GO1._position.x; //Right side of GO2 greater than left side of GO1
        bool collisionOnX = collisionX_0 && collisionX_1;
    	
    	//Check collision on y-axis
        bool collisionY_0 = GO1._position.y + GO1._size.y >= GO2._position.y; //Bottom side of GO1 greater than top side of GO2
        bool collisionY_1 = GO2._position.y + GO2._size.y >= GO1._position.y; //Bottom side of GO2 greater than top side of GO1
        bool collisionOnY = collisionY_0 && collisionY_1;

        return collisionOnX && collisionOnY;
    }

    void CheckCollisions()
    {
	    for (GameObject& box : _gameLevelCreator->_bricks)
	    {
		    if (!box._destroyed)
		    {
			    if (CollisionOccured(*_ball, box))
			    {
                    if (!box._solid)
                        box._destroyed = true;
			    }
		    }
	    }
    }
	
public:
	//Game
    GameState _state;
    bool _keys[1024];
    unsigned int _width, _height;
    SpriteRenderer* _spriteRenderer = nullptr;
    GameLevelCreator* _gameLevelCreator = nullptr;
    GameObject* _background = nullptr;
	
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
    	
        ResourceManager::DeleteTextures();
    }

    void init()
    {
    	//Create SpriteRenderer
        _spriteRenderer = new SpriteRenderer("res/shader/breakout_vs.glsl", "res/shader/breakout_fs.glsl", _width, _height);
            	
        //Create GameLevelCreator
        _gameLevelCreator = new GameLevelCreator(_spriteRenderer, _width, _height);
    	
    	//Load level from file
        _gameLevelCreator->generateLevel("res/levels/basic.level");

        //Background creation
        ResourceManager::LoadTexture("res/textures/Background_1.jpg", "Background");
        _background = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec2(_width, _height), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, ResourceManager::GetTexture("Background"), _spriteRenderer);
    	
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

        CheckCollisions();
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
        //Render backgroudn
        _background->Draw();
    	
    	//Render level
        _gameLevelCreator->renderLevel();

    	//Render player
        _player->Draw();

    	//Render ball
        _ball->Draw();
    }
};