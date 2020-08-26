#pragma once

#include "SpriteRenderer.hpp"
#include "ResourceManager.hpp"
#include "GameLevelCreator.hpp"
#include "BallObject.hpp"
#include "ParticleGenerator.hpp"
#include "Random.hpp"
#include "PowerUpManager.hpp"

enum CollisionSide
{
    LEFT,
	RIGHT,
	TOP,
	BOTTOM
};

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game
{
private:
    bool CollisionOccured(BallObject& Ball, GameObject& GO, bool isPlayer = false) const
    {
    	//Collision detection based on axis-aligned bounding boxes
        const glm::vec2 ball_center(Ball._position + Ball._radius);
        const glm::vec2 box_half(GO._size.x / 2, GO._size.y / 2);
        const glm::vec2 box_center(GO._position + box_half);
        glm::vec2 diff = ball_center - box_center;
        const glm::vec2 clamped = glm::clamp(diff, -box_half, box_half);
        const glm::vec2 closest_point = box_center + clamped;
        diff = closest_point - ball_center;                
            	
        if (glm::length(diff) < Ball._radius)
        {
        	//Which side of the quad did the ball hit
            const float left_line = GO._position.x;
            const float right_line = GO._position.x + GO._size.x;
            const float top_line = GO._position.y;
            const float bottom_line = GO._position.y + GO._size.y;

            //Repositioning
            const float penetration_depth = Ball._radius - glm::length(diff);
        	
        	if (!isPlayer) //Check for collision with boxes
        	{
                if ((int)closest_point.x == (int)left_line)
                {
                    ChangeDirection(LEFT);
                    Ball._position.x += penetration_depth;
                }                    
                else if ((int)closest_point.x == (int)right_line)
                {
                    ChangeDirection(RIGHT);
                    Ball._position.x -= penetration_depth;
                }                    
                else if ((int)closest_point.y == (int)top_line)
                {
                    ChangeDirection(TOP);
                    Ball._position.y -= penetration_depth;
                }                    
                else if ((int)closest_point.y == (int)bottom_line)
                {
                    ChangeDirection(BOTTOM);
                    Ball._position.y += penetration_depth;
                }
                    
        	}
            else         //Check for collision with player paddle
            {
				if ((int)closest_point.y == (int)top_line)
				{
					//Calculate where the ball hit the paddle and change velocity based on the distance to the center
                    const float paddle_center = GO._position.x + GO._size.x / 2.0f;
                    const float distance_to_center = (Ball._position.x + Ball._radius) - paddle_center;
                    float percentage = distance_to_center / (GO._size.x / 2.0f);
                    const float strength = 4.0f;
                    const glm::vec2 oldVelocity = Ball._velocity;
                    Ball._velocity.x = oldVelocity.x + distance_to_center * strength;

					//Change velocity in the y-direction
                    ChangeDirection(TOP);

					//Normalize velocity
                    Ball._velocity = glm::normalize(Ball._velocity) * glm::length(oldVelocity);

					//Reset ball after hit based on penetration depth
                    Ball._position.y -= penetration_depth;
				}					
            }     
        	
            return true;
        }
            
        return false;
    }

    void ChangeDirection(CollisionSide side) const
    {
        if (side == TOP || side == BOTTOM)
            _ball->_velocity.y = -_ball->_velocity.y;
        else
            _ball->_velocity.x = -_ball->_velocity.x;
    }
	
    void CheckCollisions() const
    {
	    for (GameObject& box : _gameLevelCreator->_bricks)
	    {
		    if (!box._destroyed)
		    {
			    if (CollisionOccured(*_ball, box))
			    {
                    if (!box._solid)
                    {
                        box._destroyed = true;                    	
                    }                    
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
    glm::vec2 _lastPos;

    //Particles
    ParticleGenerator* _particleGenerator = nullptr;

	//PowerUps
    PowerUpManager* _powerUpManager = nullptr;
	
    Game(unsigned int width, unsigned int height)
        : _state(GAME_ACTIVE), _keys(), _width(width), _height(height), _playerSize(140.0f, 20.0f), _playerVelocity(500.0f), _ballVelocity((random::Float() * 400.0f) - 200.0f, -400.0f), _ballRadius(15.0f)
    {
    	
    }

    ~Game()
    {
        delete _spriteRenderer;
        delete _gameLevelCreator;
        delete _player;
        delete _ball;
        delete _particleGenerator;
        delete _powerUpManager;
    	
        ResourceManager::DeleteTextures();
        ResourceManager::DeleteShaders();
    }

    void init()
    {        
    	//Create SpriteRenderer
        ResourceManager::LoadShader("res/shader/breakout/breakout_vs.glsl", "res/shader/breakout/breakout_fs.glsl", "Box_Shader");
        _spriteRenderer = new SpriteRenderer(ResourceManager::GetShader("Box_Shader"), _width, _height);
            	
        //Create GameLevelCreator
        ResourceManager::LoadTexture("res/textures/Block.jpg", "Block");
        ResourceManager::LoadTexture("res/textures/Block_solid.jpg", "Block_solid");
        _gameLevelCreator = new GameLevelCreator(_spriteRenderer, _width, _height, ResourceManager::GetTexture("Block"), ResourceManager::GetTexture("Block_solid"));
    	
    	//Load level from file
        _gameLevelCreator->generateLevel("res/levels/basic.level");

        //Background creation
        ResourceManager::LoadTexture("res/textures/Background_1.jpg", "Background");
        _background = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec2(_width, _height), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, ResourceManager::GetTexture("Background"), _spriteRenderer);
    	
        //Player paddle creation 	
        ResourceManager::LoadTexture("res/textures/Paddle.png", "Paddle");
        _player = new GameObject(glm::vec2(_width / 2.0f - _playerSize.x / 2.0f, _height - _playerSize.y), _playerSize, glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, ResourceManager::GetTexture("Paddle"), _spriteRenderer);

        //Ball creation
        ResourceManager::LoadTexture("res/textures/Cannonball_SW.png", "Ball");
        _ball = new BallObject(_player->_position + glm::vec2(_player->_size.x / 2.0f - _ballRadius, -_ballRadius * 2.0f), _ballRadius, _ballVelocity, glm::vec3(0.7f, 0.7f, 1.0f), ResourceManager::GetTexture("Ball"), _spriteRenderer);
        _lastPos = _ball->_position;
    	
        //ParticleGenerator creation
        ResourceManager::LoadShader("res/shader/breakout/particle_vs.glsl", "res/shader/breakout/particle_fs.glsl", "Particle_Shader");
        ResourceManager::LoadTexture("res/textures/Particle.png", "Particle");
        _particleGenerator = new ParticleGenerator(ResourceManager::GetShader("Particle_Shader"), ResourceManager::GetTexture("Particle"),_spriteRenderer->getProjectionMatrix());
        _particleGenerator->createParticles(glm::vec2(_ball->_position.x + 7.5f, _ball->_position.y + 7.5f), glm::vec2(10.0f, 10.0f), glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));

        //PowerUp creation
        ResourceManager::LoadTexture("res/textures/Powerup_speed.png", "Speed");
        ResourceManager::LoadTexture("res/textures/Powerup_sticky.png", "Sticky");
        ResourceManager::LoadTexture("res/textures/Powerup_passthrough.png", "PassThrough");
        ResourceManager::LoadTexture("res/textures/Powerup_increase.png", "Increase");
        _powerUpManager = new PowerUpManager(ResourceManager::GetTexture("Speed"), ResourceManager::GetTexture("Sticky"), ResourceManager::GetTexture("PassThrough"), ResourceManager::GetTexture("Increase"), _spriteRenderer);
    }

    void update(float dt)
    {
    	//Move ball via velocity vector
        _ball->Move(dt, _width);

    	//Check for collisions with boxes
        CheckCollisions();
    	
        //Check for collision with player paddle
        CollisionOccured(*_ball, *_player, true);

    	//Update Particles
        _particleGenerator->updateParticles(dt, glm::vec2(_ball->_position.x + 7.5f, _ball->_position.y + 7.5f));

    	//Update all active powerups
        _powerUpManager->updatePowerUps(dt);
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
        //Render background
        _background->Draw();
    	
    	//Render level
        _gameLevelCreator->renderLevel();

    	//Render player
        _player->Draw();
    	
        //Render particles if ball position changed       
        if(_lastPos != _ball->_position)
			_particleGenerator->renderParticles();

    	//Update last ball position
        _lastPos = _ball->_position;
    	
    	//Render ball
        _ball->Draw();

    	//Render powerups
        _powerUpManager->renderPowerUps();
    }
};