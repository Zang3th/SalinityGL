#pragma once

#include "AssimpLoader.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Standardmodel.hpp"

class GrassEntity
{
private:
	AssimpLoader* _grassData;
	Texture* _grassTexture;
	unsigned int _grassTextureCount;
	Shader* _grassShader;
	std::vector<Standardmodel*> _grassModels;
	friend class EntityManager;

public:
	GrassEntity(const char* objFile, const char* texture, unsigned int* nextTextureSlot)
	{
		//Create the data
		_grassData = new AssimpLoader(objFile);

		//Create the texture
		_grassTextureCount = *nextTextureSlot;
		_grassTexture = new Texture(texture, _grassTextureCount);
		(*nextTextureSlot)++;

		//Create the shader
		_grassShader = new Shader("res/shader/standard_vs.glsl", "res/shader/standard_fs.glsl");
	}

	~GrassEntity()
	{
		delete _grassData;
		delete _grassTexture;
		delete _grassShader;

		for (auto obj : _grassModels)
			delete obj;
	}

	void addGrass()
	{
		Standardmodel* grassmodel = new Standardmodel(_grassData, _grassShader, _grassTextureCount, false);
		_grassModels.push_back(grassmodel);
	}

	void translate(const unsigned int& grassID, const glm::vec3& tVec3) const
	{
		_grassModels.at(grassID)->translate(tVec3);
	}

	void rotate(const unsigned int& grassID, const float& angle, const glm::vec3& axis) const
	{
		_grassModels.at(grassID)->rotate(angle, axis);
	}

	void scale(const unsigned int& grassID, const glm::vec3& scalar) const
	{
		_grassModels.at(grassID)->scale(scalar);
	}
};