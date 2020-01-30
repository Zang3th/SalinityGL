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

public:
	Model* _playermodel;

	Player(Model* player, GroundData* ground_data)
		: _playermodel(player), _ground_data(ground_data)
	{
		
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

		_playermodel->rotate(_currentTurnSpeed * deltaTime, glm::vec3(0, 1, 0));
		float distance = _currentSpeed * deltaTime;
		float dx = distance * glm::sin(glm::radians(_playermodel->_rotation));
		float dz = distance * glm::cos(glm::radians(_playermodel->_rotation));
		//float dy = _ground_data->getHeightValue(dx / 2, dz / 2, 6.0);
		_playermodel->translate(glm::vec3(dx, 0, dz));
	}
};