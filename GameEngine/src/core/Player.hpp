#pragma once

class Model;

enum Player_Movement
{
	P_FORWARD,
	P_BACKWARD,
	P_LEFT,
	P_RIGHT
};

//Default Playervalues
const float P_SPEED = 5.0f;

class Player
{
private:
	Model* _playermodel;
	float _movementspeed;

public:
	Player(Model* player)
		: _playermodel(player), _movementspeed(P_SPEED)
	{
		
	}

	//Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Player_Movement direction, float deltaTime)
	{
		float velocity = _movementspeed * deltaTime;

		if (direction == P_FORWARD)
			std::cout << "forward\n";
			//Position += Front * velocity;
		if (direction == P_BACKWARD)
			std::cout << "backward\n";
			//Position -= Front * velocity;
		if (direction == P_LEFT)
			std::cout << "left\n";
			//Position -= Right * velocity;
		if (direction == P_RIGHT)
			std::cout << "right\n";
			//Position += Right * velocity;		

		//std::cout << Position.x << ", " << Position.y << ", " << Position.z << std::endl;
	}
};