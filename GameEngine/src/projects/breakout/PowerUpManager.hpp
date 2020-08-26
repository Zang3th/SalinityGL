#pragma once

#include <vector>
#include "PowerUpObject.hpp"

class PowerUpManager
{
private:
	Texture* _speedTexture, * _stickyTexture, * _passThroughTexture, * _padIncreaseTexture;
	SpriteRenderer* _spriteRenderer;
	
	void createPowerUps()
	{
		_powerUps.emplace_back(glm::vec2(100.0f, 100.0f), glm::vec3(0.219f, 0.686f, 1.0f), _speedTexture, _spriteRenderer, "speed", 6.0f);
		_powerUps.emplace_back(glm::vec2(300.0f, 100.0f), glm::vec3(0.988f, 0.588f, 0.152f), _stickyTexture, _spriteRenderer, "sticky", 0.0f);
		_powerUps.emplace_back(glm::vec2(500.0f, 100.0f), glm::vec3(0.878f, 0.023, 0.129), _passThroughTexture, _spriteRenderer, "passThrough", 8.0f);
		_powerUps.emplace_back(glm::vec2(700.0f, 100.0f), glm::vec3(0.023f, 0.878f, 0.4f), _padIncreaseTexture, _spriteRenderer, "padIncrease", 8.0f);
	}
	
public:
	std::vector<PowerUpObject> _powerUps;
	
	PowerUpManager(Texture* tex1, Texture* tex2, Texture* tex3, Texture* tex4, SpriteRenderer* spriteRenderer)
		: _speedTexture(tex1), _stickyTexture(tex2), _passThroughTexture(tex3), _padIncreaseTexture(tex4), _spriteRenderer(spriteRenderer)
	{
		this->createPowerUps();
	}

	//Function gets called for every destroyed block -> spawn powerups based on a certain chance
	void spawnPowerUps(glm::vec2 pos)
	{
		
	}

	//Move all active PowerUps
	void updatePowerUps(float dt)
	{
		for (auto& power : _powerUps)
		{
			power._position += power._velocity * dt;
		}
	}

	//Render all active PowerUps
	void renderPowerUps()
	{
		for (PowerUpObject power : _powerUps)
		{
			power.Draw();
		}
	}
};