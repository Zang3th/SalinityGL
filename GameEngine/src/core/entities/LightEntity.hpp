#pragma once

#include "AssimpLoader.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Standardmodel.hpp"
#include "Lightmodel.hpp"

class LightEntity
{
private:
	AssimpLoader* _lanternData, * _lightbulbData;
	Texture* _lanternTex, * _lightbulbTex;
	unsigned int _lanternTexCount, _lightbulbTexCount;
	Shader* _lanternShader, * _lightbulbShader;
	Standardmodel* _lanternModel;
	Lightmodel* _lightbulbModel;
	friend class EntityManager;
	
public:
	LightEntity(const char* lanternOBJ, const char* lanternTexture, const char* lightbulbOBJ, const char* lightbulbTexture, unsigned int* nextTextureSlot)
	{
		//Create the data
		_lanternData = new AssimpLoader(lanternOBJ);
		_lightbulbData = new AssimpLoader(lightbulbOBJ);

		//Create the textures
		_lanternTexCount = *nextTextureSlot;
		_lanternTex = new Texture(lanternTexture, _lanternTexCount);
		(*nextTextureSlot)++;

		_lightbulbTexCount = *nextTextureSlot;
		_lightbulbTex = new Texture(lightbulbTexture, _lightbulbTexCount);
		(*nextTextureSlot)++;

		//Create the shaders
		_lanternShader = new Shader("res/shader/standard_vs.glsl", "res/shader/standard_fs.glsl");
		_lightbulbShader = new Shader("res/shader/lightbulb_vs.glsl", "res/shader/lightbulb_fs.glsl");

		//Combine everything to the model
		_lanternModel = new Standardmodel(_lanternData, _lanternShader, _lanternTexCount);
		_lightbulbModel = new Lightmodel(_lightbulbData, _lightbulbShader, _lightbulbTexCount, glm::vec3(1.0, 1.0, 1.0));		
	}

	~LightEntity()
	{
		delete _lanternData;
		delete _lightbulbData;
		delete _lanternTex;
		delete _lightbulbTex;
		delete _lanternShader;
		delete _lightbulbShader;
		delete _lanternModel;
		delete _lightbulbModel;
	}

	void translate(const glm::vec3& tVec3) const
	{
		_lanternModel->translate(tVec3);
		_lightbulbModel->translate(tVec3);			
	}
	
	void rotate(const float& angle, const glm::vec3& axis) const
	{
		_lanternModel->rotate(angle, axis);
		_lightbulbModel->rotate(angle, axis);		
	}

	void scale(const glm::vec3& scalar) const
	{
		_lanternModel->scale(scalar);
		_lightbulbModel->scale(scalar);		
	}
};