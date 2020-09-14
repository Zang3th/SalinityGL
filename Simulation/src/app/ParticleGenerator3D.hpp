#pragma once

#include "Object.hpp"
#include "ResourceManager.hpp"
#include "Random.hpp"

const unsigned int _numberOfParticles = 10000;

class ParticleGenerator3D 
{
private:
	//Actual particle data -> only once
	struct ParticleObject
	{
		Texture* _texture = nullptr;
		Shader* _shader = nullptr;
		Data* _data = nullptr;
		VertexBuffer* _vbo1 = nullptr, * _vbo2 = nullptr, * _vbo3 = nullptr, * _vbo4 = nullptr;
		VertexArray* _vao = nullptr;
		IndexBuffer* _ib = nullptr;
		glm::mat4 _model, _view, _projection;
		unsigned int _vertices;
		
		ParticleObject(Texture* texture, Shader* shader, Data* data)
			: _texture(texture), _shader(shader), _data(data)
		{
			
		}
	};

	//Particle instances -> as many as you want
	struct ParticleInstance
	{
		glm::vec3 _position, _velocity, _startColor, _endColor, _currentColor;
		float _size, _lifeTime, _lifeRemaining;
	};

	ParticleObject* _particleObject;	
	glm::vec3 _particleTranslations[_numberOfParticles];	
	
	void initData()
	{
		//Allocate resources
		ResourceManager::LoadShader("../res/shader/simulation/object_instanced_vs.glsl", "../res/shader/simulation/object_instanced_fs.glsl", "Object_shader");
		ResourceManager::LoadTexture("../res/textures/models/Moon.jpg", "Sphere_texture");
		ResourceManager::LoadData("../res/obj/geometry/sphere.obj", "Sphere_data");

		//Create object
		_particleObject = new ParticleObject(ResourceManager::GetTexture("Sphere_texture"), ResourceManager::GetShader("Object_shader"), ResourceManager::GetData("Sphere_data"));
		
		//Create and bind vao
		_particleObject->_vao = new VertexArray();
		_particleObject->_vao->bind();

		//Create color buffer
		std::vector<glm::vec3> color;
		for (int i = 0; i < _particleObject->_data->_vertices.size(); i++)
		{
			color.emplace_back(glm::vec3(random::Float() * 0.5f, random::Float() * 0.5f, random::Float() * 0.5f));
		}

		//Create translation buffer
		for (int i = 0; i < _numberOfParticles; i++)
		{
			_particleTranslations[i] = glm::vec3(random::Float() * 1000.0f, random::Float() * 300.0f, random::Float() * 1000.0f);
		}
		
		//Create vbo's and configure vao
		//vbo1 (vertice data)
		_particleObject->_vbo1 = new VertexBuffer(&_particleObject->_data->_vertices[0], _particleObject->_data->_vertices.size() * sizeof(glm::vec3));
		_particleObject->_vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		//vbo2 (texture coordinates)
		_particleObject->_vbo2 = new VertexBuffer(&_particleObject->_data->_texCoords[0], _particleObject->_data->_texCoords.size() * sizeof(glm::vec2));
		_particleObject->_vao->DefineAttributes(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
		//vbo3 (colors)
		_particleObject->_vbo3 = new VertexBuffer(&color[0], color.size() * sizeof(glm::vec3));
		_particleObject->_vao->DefineAttributes(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		_particleObject->_vao->AttributeDivisor(2, 1);
		//vbo4 (translations)
		_particleObject->_vbo4 = new VertexBuffer(&_particleTranslations[0], _numberOfParticles * sizeof(glm::vec3));
		_particleObject->_vao->DefineAttributes(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		_particleObject->_vao->AttributeDivisor(3, 1);
		
		//Create ib
		_particleObject->_ib = new IndexBuffer(&_particleObject->_data->_indices[0], _particleObject->_data->_indices.size() * sizeof(glm::uvec3));

		//Calculate vertices to render
		_particleObject->_vertices = _particleObject->_data->_indices.size() * 3;
		_verticsToRender = _particleObject->_vertices * _numberOfParticles;
		
		//Unbind vao and vbo
		_particleObject->_vbo1->unbind();
		_particleObject->_vbo2->unbind();
		_particleObject->_vbo3->unbind();
		_particleObject->_vbo4->unbind();
		_particleObject->_vao->unbind();
	}
	
public:
	unsigned int _verticsToRender;
	
	void init()
	{
		initData();
	}

	void render()
	{
		//Matrices
		_particleObject->_projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		_particleObject->_view = camera.GetViewMatrix();
		_particleObject->_model = glm::mat4(1.0f);
		
		_particleObject->_shader->bind();

		//Set uniforms
		_particleObject->_shader->SetUniformMat4f("model", _particleObject->_model);
		_particleObject->_shader->SetUniformMat4f("projection", _particleObject->_projection);
		_particleObject->_shader->SetUniformMat4f("view", _particleObject->_view);

		//Set texture
		_particleObject->_texture->bind();

		//Bind vao
		_particleObject->_vao->bind();
		
		glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)_particleObject->_vertices, GL_UNSIGNED_INT, nullptr, _numberOfParticles);
	}
};