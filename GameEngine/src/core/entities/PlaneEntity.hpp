#pragma once

#include "PlaneData.hpp"

class PlaneEntity
{
private:
	PlaneData* _planeData;
	Texture* _texture;
	unsigned int _textureCount;
	Shader* _shader;
	Standardmodel* _planeModel;
	friend class EntityManager;
	
public:
	PlaneEntity(const unsigned int& size, const unsigned int& tileSize, const char* texture, const char* vShader, const char* fShader, unsigned int* nextTextureSlot)
	{
		//Create the data
		_planeData = new PlaneData(size, tileSize);

		//Create the texture
		_textureCount = *nextTextureSlot;
		_texture = new Texture(texture, _textureCount);
		(*nextTextureSlot)++;

		//Create the shader
		_shader = new Shader(vShader, fShader);

		//Combine everything to the model
		_planeModel = new Standardmodel(_planeData, _shader, _textureCount, false);
	}

	~PlaneEntity()
	{
		delete _planeData;
		delete _texture;
		delete _shader;
		delete _planeModel;
	}
	
	void translate(const glm::vec3& tVec3) const
	{
		_planeModel->translate(tVec3);
	}

	void rotate(const float& angle, const glm::vec3& axis) const
	{
		_planeModel->rotate(angle, axis);
	}

	void scale(const glm::vec3& scalar) const
	{
		_planeModel->scale(scalar);
	}
};