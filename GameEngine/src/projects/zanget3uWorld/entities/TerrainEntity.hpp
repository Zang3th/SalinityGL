#pragma once

#include "GroundData.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Groundmodel.hpp"

class TerrainEntity
{	
private:
	GroundData* _groundData;
	Texture* _terrainTex, * _pathwayTex, * _blendmapTex;
	unsigned int _terrainTexCount, _pathwayTexCount, _blendmapTexCount;
	Shader* _shader;
	Groundmodel* _groundModel;	
	friend class EntityManager;
	
	void colorPickedVertice(const glm::vec3& terrainEntry, unsigned int* lastIndex) const
	{
		//Delete color of the last picked vertice
		((RawData*)_groundData)->_isPicked.at(*lastIndex) = glm::vec3(1.0, 1.0, 1.0);

		//Get the picked vertice position
		int x = (int)terrainEntry.x;
		int z = (int)terrainEntry.z;

		//Error-Checking
		if (x < 0)
			x = 0;
		if (x > 511)
			x = 511;
		if (z < 1)
			z = 1;
		if (z > 511)
			z = 511;

		//Color new vertice
		unsigned int index = _groundData->_twoDimArray[x][z];
		((RawData*)_groundData)->_isPicked.at(index) = glm::vec3(255, 0, 0);
		_groundModel->updateColorOfPickedVertice();
		*lastIndex = index;
	}

	void deleteLastPickedVertice(unsigned int* lastIndex) const
	{
		((RawData*)_groundData)->_isPicked.at(*lastIndex) = glm::vec3(1.0, 1.0, 1.0);
		_groundModel->updateColorOfPickedVertice();
	}

	void raise(unsigned int* lastIndex) const
	{
		((RawData*)_groundData)->_vertices.at(*lastIndex).y += 0.05;
		_groundModel->updateHeightOfPickedVertice();
	}

	void sink(unsigned int* lastIndex) const
	{
		((RawData*)_groundData)->_vertices.at(*lastIndex).y -= 0.05;
		_groundModel->updateHeightOfPickedVertice();
	}

public:	
	TerrainEntity(const unsigned int& size, const unsigned int& tileSize, const char* heightmap, const char* terrainTexture, const char* pathwayTexture, const char* blendmap, unsigned int* nextTextureSlot)
	{		
		//Create the data
		_groundData = new GroundData(size, tileSize, heightmap);

		//Create 3 textures
		_terrainTexCount = *nextTextureSlot;
		_terrainTex = new Texture(terrainTexture, _terrainTexCount);
		(*nextTextureSlot)++;

		_pathwayTexCount = *nextTextureSlot;
		_pathwayTex = new Texture(pathwayTexture, _pathwayTexCount);
		(*nextTextureSlot)++;

		_blendmapTexCount = *nextTextureSlot;
		_blendmapTex = new Texture(blendmap, _blendmapTexCount);
		(*nextTextureSlot)++;
		
		//Create the shader
		_shader = new Shader("res/shader/zanget3uWorld/ground_vs.glsl", "res/shader/zanget3uWorld/ground_fs.glsl");

		//Combine everything to the model
		_groundModel = new Groundmodel(_groundData, _shader, _terrainTexCount, _pathwayTexCount, _pathwayTexCount, _blendmapTexCount);
	}

	~TerrainEntity()
	{		
		delete _groundData;
		delete _terrainTex;
		delete _pathwayTex;
		delete _blendmapTex;
		delete _shader;
		delete _groundModel;				
	}

	void translate(const glm::vec3& tVec3) const
	{
		_groundModel->translate(tVec3);
	}

	void rotate(const float& angle, const glm::vec3& axis) const
	{
		_groundModel->rotate(angle, axis);
	}

	void scale(const glm::vec3& scalar) const
	{
		_groundModel->scale(scalar);
	}

	float getHeightAt(const float& x, const float& z) const
	{
		return _groundData->getHeightValueUnbuffered((int)x, (int)z);
	}
};