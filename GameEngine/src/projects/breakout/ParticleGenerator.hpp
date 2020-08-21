#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct Particle
{
	glm::vec2 _position, _velocity;
	glm::vec4 _color;
	float _life;

	Particle()
		: _position(0.0f), _velocity(0.0f), _color(1.0f), _life(0.0f)
	{
		
	}
};

class ParticleGenerator
{
public:
	
	ParticleGenerator()
	{

	}

	~ParticleGenerator()
	{

	}
};