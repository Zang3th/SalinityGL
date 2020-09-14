#pragma once

#include <vector>
#include "PowerUpObject.hpp"

class PowerUpManager
{
private:
	Texture* _speedTexture, * _stickyTexture, * _passThroughTexture, * _padIncreaseTexture;
	SpriteRenderer* _spriteRenderer;
		
public:
	std::vector<PowerUpObject> _powerUpsToRender;
	
	PowerUpManager(Texture* tex1, Texture* tex2, Texture* tex3, Texture* tex4, SpriteRenderer* spriteRenderer)
		: _speedTexture(tex1), _stickyTexture(tex2), _passThroughTexture(tex3), _padIncreaseTexture(tex4), _spriteRenderer(spriteRenderer)
	{
		
	}

	//Function gets called for every destroyed block -> spawn powerups based on a certain chance
	void spawnPowerUps(glm::vec2 pos, unsigned int proc_chance)
	{
		if (proc_chance == 0)
		{
			_powerUpsToRender.emplace_back(pos, glm::vec3(0.219f, 0.686f, 1.0f), _speedTexture, _spriteRenderer, "speed", 6.0f);
		}
		else if (proc_chance == 1)
		{
			_powerUpsToRender.emplace_back(pos, glm::vec3(0.988f, 0.588f, 0.152f), _stickyTexture, _spriteRenderer, "sticky", 0.0f);
		}
		else if (proc_chance == 2)
		{
			_powerUpsToRender.emplace_back(pos, glm::vec3(0.878f, 0.023, 0.129), _passThroughTexture, _spriteRenderer, "passThrough", 8.0f);
		}
		else if (proc_chance == 3)
		{
			_powerUpsToRender.emplace_back(pos, glm::vec3(0.023f, 0.878f, 0.4f), _padIncreaseTexture, _spriteRenderer, "padIncrease", 8.0f);
		}
	}

	//Move all PowerUps
	void updatePowerUps(float dt)
	{
		unsigned int index = 0;
		for (auto& power : _powerUpsToRender)
		{
			power._position += power._velocity * dt;
			if (power._position.y > 1200)
			{
				_powerUpsToRender.erase(_powerUpsToRender.begin() + index);
			}
			index++;
		}
	}

	//Render all PowerUps
	void renderPowerUps()
	{
		for (PowerUpObject power : _powerUpsToRender)
		{
			power.Draw();
		}
	}
};