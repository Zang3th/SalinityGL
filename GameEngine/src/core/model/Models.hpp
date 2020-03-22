#pragma once

#include "Renderer.hpp"
#include "Terrain.hpp"

class Models
{
private:
	std::vector<Basemodel*> _models;
	std::vector<Terrain*> _terrains;	
	Renderer _renderer;
	
public:
	Models()
	{
		
	}

	~Models()
	{
		for (auto t : _terrains)
			delete t;
	}

	void addTerrain(const unsigned int& size, const unsigned int& tileSize, const char* heightmap, const char* terrainTexture, const char* pathwayTexture, const char* blendmap, const char* vsShader, const char* fsShader)
	{
		Terrain* terrain = new Terrain(size, tileSize, heightmap, terrainTexture, pathwayTexture, blendmap, vsShader, fsShader);
		_terrains.push_back(terrain);
		_models.push_back(terrain->getBasemodel());
	}
	
	void addObjModel(const char* objFile, const char* texture, const char* vsShader, const char* fsShader)
	{
		
	}
	
	void render()
	{
		_renderer.prepare();
		_renderer.render(_models);
	}
};