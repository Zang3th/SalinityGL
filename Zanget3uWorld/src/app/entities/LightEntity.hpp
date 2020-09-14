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
	std::vector<Standardmodel*> _lanternModels;
	std::vector<Lightmodel*> _lightbulbModels;
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
		_lanternShader = new Shader("../res/shader/zanget3uWorld/standard_vs.glsl", "../res/shader/zanget3uWorld/standard_fs.glsl");
		_lightbulbShader = new Shader("../res/shader/zanget3uWorld/lightbulb_vs.glsl", "../res/shader/zanget3uWorld/lightbulb_fs.glsl");			
	}

	~LightEntity()
	{
		delete _lanternData;
		delete _lightbulbData;
		delete _lanternTex;
		delete _lightbulbTex;
		delete _lanternShader;
		delete _lightbulbShader;

		for (auto lantern : _lanternModels)
			delete lantern;

		for (auto lightbulb : _lightbulbModels)
			delete lightbulb;
	}

	void addLight()
	{
		Standardmodel* lanternModel = new Standardmodel(_lanternData, _lanternShader, _lanternTexCount);
		Lightmodel* lightbulbModel = new Lightmodel(_lightbulbData, _lightbulbShader, _lightbulbTexCount, glm::vec3(1.0, 1.0, 1.0));
		_lanternModels.push_back(lanternModel);
		_lightbulbModels.push_back(lightbulbModel);
	}
	
	void translate(const unsigned int& lightID, const glm::vec3& tVec3)
	{
		_lanternModels.at(lightID)->translate(tVec3);
		_lightbulbModels.at(lightID)->translate(tVec3);
	}	
	
	void rotate(const unsigned int& lightID, const float& angle, const glm::vec3& axis) const
	{
		_lanternModels.at(lightID)->rotate(angle, axis);
		_lightbulbModels.at(lightID)->rotate(angle, axis);		
	}

	void scale(const unsigned int& lightID, const glm::vec3& scalar) const
	{
		_lanternModels.at(lightID)->scale(scalar);
		_lightbulbModels.at(lightID)->scale(scalar);
	}
};