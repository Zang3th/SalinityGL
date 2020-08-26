#pragma once

#include "GameObject.hpp"

class PowerUpObject : public GameObject
{
public:
	//Object state
	std::string _type;
	float       _duration;
	bool        _activated;

	//Constructor
	PowerUpObject(glm::vec2 pos, glm::vec3 color, Texture* spriteTexture, SpriteRenderer* spriteRenderer, const std::string& type, float duration)
		: GameObject(pos, glm::vec2(140.0f, 40.0f), glm::vec2(0.0f, 150.0f), color, 0.0f, spriteTexture, spriteRenderer), _type(type), _duration(duration), _activated(false)
	{
		
	}
};
