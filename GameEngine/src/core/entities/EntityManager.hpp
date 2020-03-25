#pragma once

#include "Renderer.hpp"
#include "TerrainEntity.hpp"
#include "ObjmodelEntity.hpp"
#include "CubemapEntity.hpp"
#include "PlaneEntity.hpp"
#include "TreeEntity.hpp"
#include "LightEntity.hpp"
#include "RayEntity.hpp"

class EntityManager
{
private:
	Renderer _renderer;
	unsigned int _nextTextureSlot = 0;	
	std::vector<Basemodel*> _models;
	unsigned int _lastIndex = 0;
	
	std::vector<ObjmodelEntity*> _objs;
	std::vector<PlaneEntity*> _planes;
	std::vector<TreeEntity*> _trees;
	std::vector<LightEntity*> _lights;
	TerrainEntity* _terrain;
	CubemapEntity* _cubemap;
	RayEntity* _ray;
	
public:
	
	EntityManager()
	{
		
	}

	~EntityManager()
	{
		for (auto objs : _objs)
			delete objs;

		for (auto planes : _planes)
			delete planes;

		for (auto trees : _trees)
			delete trees;

		for (auto lights : _lights)
			delete lights;

		delete _terrain;
		delete _ray;
		delete _cubemap;
	}

	TerrainEntity* addTerrainEntity(const unsigned int& size, const unsigned int& tileSize, const char* heightmap, const char* terrainTexture, const char* pathwayTexture, const char* blendmap)
	{
		_terrain = new TerrainEntity(size, tileSize, heightmap, terrainTexture, pathwayTexture, blendmap, &_nextTextureSlot);
		_models.push_back((Basemodel*)_terrain->_groundModel);
		return _terrain;
	}
	
	ObjmodelEntity* addOBJEntity(const char* objFile, const char* texture)
	{
		ObjmodelEntity* obj = new ObjmodelEntity(objFile, texture, &_nextTextureSlot);
		_objs.push_back(obj);
		_models.push_back((Basemodel*)obj->_standardmodel);
		return obj;
	}

	PlaneEntity* addPlaneEntity(const unsigned int& size, const unsigned int& tileSize, const char* texture)
	{
		PlaneEntity* plane = new PlaneEntity(size, tileSize, texture, &_nextTextureSlot);
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

	LightEntity* addLightEntity(const char* lanternOBJ, const char* lanternTexture, const char* lightbulbOBJ, const char* lightbulbTexture)
	{
		LightEntity* light = new LightEntity(lanternOBJ, lanternTexture, lightbulbOBJ, lightbulbTexture, &_nextTextureSlot);
		_lights.push_back(light);
		_models.push_back((Basemodel*)light->_lanternModel);
		_models.push_back((Basemodel*)light->_lightbulbModel);
		return light;
	}
	
	void addCubemapEntity()
	{
		_cubemap = new CubemapEntity(&_nextTextureSlot);
		_models.push_back((Basemodel*)_cubemap->_standardmodel);
	}

	void createRay()
	{
		_ray = new RayEntity();
		_models.push_back((Basemodel*)_ray->_quaderModel);
	}
	
	void visualizeRay(const glm::vec3& camPosition, const glm::vec3& endPosition, const float& angle, bool& renderRay, const float& rayLength, const float& rayThickness) const
	{
		_ray->updateRay(camPosition, endPosition, angle, renderRay, rayLength, rayThickness);		
	}

	void colorPickedVertices(const glm::vec3& terrainEntry)
	{
		unsigned int tempIndex = _lastIndex;
		_lastIndex = _terrain->colorPickedVertices(terrainEntry, tempIndex);
	}
	
	void render()
	{
		_renderer.prepare();
		_renderer.render(_models);
	}
};