#pragma once

#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Random.hpp"
#include <glm/gtx/compatibility.hpp>

class ParticleGenerator
{
private:
	struct Particle
	{
		glm::vec2 _position, _velocity, _size;
		glm::vec4 _startColor, _endColor, _currentColor;
		float _lifeTime, _lifeRemaining;

		Particle(glm::vec2 position, glm::vec2 velocity, glm::vec2 size, glm::vec4 startColor, glm::vec4 endColor, float life)
			: _position(position), _velocity(velocity), _size(size), _startColor(startColor), _endColor(endColor), _lifeTime(life), _lifeRemaining(life)
		{
			
		}
	};

	void initRenderData()
	{
		//Erstellt und bindet VAO
		_vao = new VertexArray();
		_vao->bind();

		float vertices[] =
		{
			0.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,

			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
		};

		_vbo = new VertexBuffer(vertices, sizeof(vertices));
		_vao->DefineAttributes(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

		_vbo->unbind();
		_vao->unbind();
	}	

	void createParticles()
	{
		for (int i = 0; i < 500; i++)
		{			
			float xVelocity = (random::Float() * 150.0f) - 75.0f;
			float yVelocity = (random::Float() * 300.0f) - 300.0f;			
			glm::vec2 velocity(xVelocity, yVelocity);

			float lifeTime = random::Float() * 3.0f;
			
			_particles.emplace_back(glm::vec2(900.0f, 600.0f), velocity, glm::vec2(2.0f, 2.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), lifeTime);
		}		
	}
	
	VertexArray* _vao = nullptr;
	VertexBuffer* _vbo = nullptr;
	Shader* _shader = nullptr;
	glm::mat4 _projection;
	std::vector<Particle> _particles;
	
public:	
	ParticleGenerator(Shader* shader, glm::mat4 projectionMatrix)
		: _shader(shader), _projection(projectionMatrix)
	{
		this->initRenderData();
		this->createParticles();
	}

	~ParticleGenerator()
	{
		delete _vao;
		delete _vbo;
	}

	void updateParticle(float dt, glm::vec2 position)
	{
		for (int i = 0; i < _particles.size(); i++)
		{			
			Particle& p = _particles[i];	
			
			p._lifeRemaining -= dt;
			
			if (p._lifeRemaining > 0.0f)
			{
				p._position -= p._velocity * dt;
				float life = p._lifeRemaining / p._lifeTime;
				p._currentColor = glm::lerp(p._endColor, p._startColor, life);
			}
			else if (p._lifeRemaining < 0.0f)
			{
				p._position = position;
				p._lifeRemaining = p._lifeTime;
				p._currentColor = p._startColor;
			}
		}	
	}

	void renderParticle()
	{
		for (Particle p : _particles)
		{
			if (p._lifeRemaining > 0.0f)
			{
				_shader->bind();

				//Model transformations
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(p._position, 0.0f)); //translate
				model = glm::scale(model, glm::vec3(p._size, 1.0f)); //scale

				//Set Uniforms
				_shader->SetUniformMat4f("projection", _projection);
				_shader->SetUniformMat4f("model", model);
				_shader->SetUniformVec3("particleColor", p._currentColor);

				//Bind vao
				_vao->bind();

				//Render quad
				GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
			}			
		}
	}
};