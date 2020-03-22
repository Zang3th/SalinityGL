#pragma once

#include "GroundData.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Groundmodel.hpp"
#include "Basemodel.hpp"

class Terrain
{	
private:
	unsigned int _texCount = 0;
	GroundData* _groundData;
	Groundmodel* _groundModel;
	Texture* _terrainTex, * _pathwayTex, * _blendmapTex;
	unsigned int _terrainTexCount, _pathwayTexCount, _blendmapTexCount;
	Shader* _shader;
	
public:	
	Terrain(const unsigned int& size, const unsigned int& tileSize, const char* heightmap, const char* terrainTexture, const char* pathwayTexture, const char* blendmap, const char* vsShader, const char* fsShader)		
	{		
		//Create the data
		_groundData = new GroundData(size, tileSize, heightmap);

		//Create 3 textures
		_terrainTex = new Texture(terrainTexture);
		_pathwayTex = new Texture(pathwayTexture);
		_blendmapTex = new Texture(blendmap);

		//Bind the 3 textures
		_terrainTexCount = _texCount;
		_terrainTex->bind(_terrainTexCount);
		_texCount++;
		
		_pathwayTexCount = _texCount;
		_pathwayTex->bind(_pathwayTexCount);
		_texCount++;
		
		_blendmapTexCount = _texCount;
		_blendmapTex->bind(_blendmapTexCount);
		_texCount++;

		//Create the shader
		_shader = new Shader(vsShader, fsShader);

		//Combine everything to the model
		_groundModel = new Groundmodel(_groundData, _shader, _terrainTexCount, _pathwayTexCount, _pathwayTexCount, _blendmapTexCount);
	}

	~Terrain()
	{		
		delete _groundData;
		delete _terrainTex;
		delete _pathwayTex;
		delete _blendmapTex;
		delete _shader;
		delete _groundModel;				
	}

	Basemodel* getBasemodel()
	{
		return (Basemodel*)_groundModel;
	}
};