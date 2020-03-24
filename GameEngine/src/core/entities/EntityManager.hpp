#pragma once

#include "Renderer.hpp"
#include "TerrainEntity.hpp"
#include "ObjmodelEntity.hpp"
#include "CubemapEntity.hpp"
#include "PlaneEntity.hpp"
#include "TreeEntity.hpp"

class EntityManager
{
private:
	Renderer _renderer;
	unsigned int _nextTextureSlot = 0;
	
	std::vector<Basemodel*> _models;
	std::vector<TerrainEntity*> _terrains;	
	std::vector<ObjmodelEntity*> _objs;
	std::vector<PlaneEntity*> _planes;
	std::vector<TreeEntity*> _trees;
	CubemapEntity* _cubemap;
	
public:
	EntityManager()
	{
		
	}

	~EntityManager()
	{
		for (auto objs : _objs)
			delete objs;

		for (auto terrains : _terrains)
			delete terrains;

		for (auto planes : _planes)
			delete planes;

		for (auto trees : _trees)
			delete trees;
		
		delete _cubemap;
	}

	TerrainEntity* addTerrainEntity(const unsigned int& size, const unsigned int& tileSize, const char* heightmap, const char* terrainTexture, const char* pathwayTexture, const char* blendmap, const char* vShader, const char* fShader)
	{
		TerrainEntity* terrain = new TerrainEntity(size, tileSize, heightmap, terrainTexture, pathwayTexture, blendmap, vShader, fShader, &_nextTextureSlot);
		_terrains.push_back(terrain);		
		_models.push_back((Basemodel*)terrain->_groundModel);
		return terrain;
	}
	
	ObjmodelEntity* addOBJEntity(const char* objFile, const char* texture, const char* vShader, const char* fShader)
	{
		ObjmodelEntity* obj = new ObjmodelEntity(objFile, texture, vShader, fShader, &_nextTextureSlot);
		_objs.push_back(obj);
		_models.push_back((Basemodel*)obj->_standardmodel);
		return obj;
	}

	PlaneEntity* addPlaneEntity(const unsigned int& size, const unsigned int& tileSize, const char* texture, const char* vShader, const char* fShader)
	{
		PlaneEntity* plane = new PlaneEntity(size, tileSize, texture, vShader, fShader, &_nextTextureSlot);
		_planes.push_back(plane);
		_models.push_back((Basemodel*)plane->_planeModel);
		return plane;		
	}

	TreeEntity* addTreeEntity()
	{
		TreeEntity* tree = new TreeEntity(&_nextTextureSlot);
		_trees.push_back(tree);
		_models.push_back((Basemodel*)tree->_treeModel);
		_models.push_back((Basemodel*)tree->_leafModel);
		return tree;
	}
	
	void addCubemapEntity()
	{
		_cubemap = new CubemapEntity(&_nextTextureSlot);
		_models.push_back((Basemodel*)_cubemap->_standardmodel);
	}
	
	void render()
	{
		_renderer.prepare();
		_renderer.render(_models);
	}
};
