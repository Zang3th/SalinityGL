#pragma once

#include "GameObject.hpp"

class BallObject : public GameObject
{
public:
	//Object state
	float _radius;
	bool  _stuck;

	//Constructor
	BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, glm::vec3 color, Texture* spriteTexture, SpriteRenderer* spriteRenderer)
		: GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), velocity, color, 0.0f, spriteTexture, spriteRenderer, false, false), _radius(radius), _stuck(true)
	{
		
	}

	//Move ball
	void Move(float dt, float window_width)
	{
		if (!_stuck)
		{
			//Move the ball
			_position += _velocity * dt;

			//Check window bounds		
			//Left
			if (_position.x <= 0.0f)
			{
				_velocity.x = -_velocity.x;
				_position.x = 0.0f;
			}

			//Right
			if (_position.x + _size.x >= window_width)
			{
				_velocity.x = -_velocity.x;
				_position.x = window_width - _size.x;
			}

			//Top
			if (_position.y <= 0.0f)
			{
				_velocity.y = -_velocity.y;
				_position.y = 0.0f;
			}
		}		
	}
};