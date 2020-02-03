#pragma once

#include "Model.hpp"
#include "GroundData.hpp"

class Model;

enum Player_Movement
{
	P_FORWARD,
	P_BACKWARD,
	P_LEFT,
	P_RIGHT
};

//Default Playervalues
const float P_SPEED = 10.0f;
const float P_TURNSPEED = 200.0f;

class Player
{
private:	
	float _currentSpeed = 0;
	float _currentTurnSpeed = 0;
	GroundData* _ground_data = nullptr;
	float new_height = 0, old_height = 0;

public:
	Model* _playermodel;
	float _yaw = 0.0f;
	glm::vec3 _front;
	glm::vec3 _position;

	Player(Model* player, GroundData* ground_data, glm::vec3 position)
		: _playermodel(player), _ground_data(ground_data), _position(position)
	{
		player->translate(_position);
	}
	
	//Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Player_Movement direction, float deltaTime)
	{		
		if (direction == P_FORWARD) 
		{
			_currentSpeed = P_SPEED;
		}
		else if (direction == P_BACKWARD) 
		{
			_currentSpeed = -P_SPEED;
		}
		else
		{
			_currentSpeed = 0;
		}

		if (direction == P_LEFT) 
		{
			_currentTurnSpeed = P_TURNSPEED;
		}
		else if (direction == P_RIGHT) 
		{
			_currentTurnSpeed = -P_TURNSPEED;			
		}
		else
		{
			_currentTurnSpeed = 0;
		}

		//Calculate direction and rotate player
		float angle = _currentTurnSpeed * deltaTime;		
		_playermodel->rotate(angle, glm::vec3(0, 1, 0));
		_yaw += angle;
		if(_yaw >= 360.0f)
		{
			_yaw = 0.0f;
		}
		if(_yaw <= -360.0f)
		{
			_yaw = 0.0f;
		}

		//Calculate dx/dz and keep track of player position
		float velocity = _currentSpeed * deltaTime;
		glm::vec3 front;
		front.x = glm::sin(glm::radians(_yaw));
		front.y = 0.0f;
		front.z = glm::cos(glm::radians(_yaw));				
		_front = glm::normalize(front) * velocity;
		_position += _front;

		//Calculate new height (y-value)
		old_height = _position.y;
		new_height = _ground_data->getHeightValue(_position.x / 2, _position.z / 2, 6.0f) - 39.8f;
		_position.y = new_height;

		//Translate the player
		float dx = velocity * glm::sin(glm::radians(angle));
		float dz = velocity * glm::cos(glm::radians(angle));
		float dy = new_height - old_height;
		_playermodel->translate(glm::vec3(dx, dy, dz));
	}
};