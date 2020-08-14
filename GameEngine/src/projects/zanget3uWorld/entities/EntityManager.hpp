#pragma once

#include "Renderer.hpp"
#include "TerrainEntity.hpp"
#include "ObjmodelEntity.hpp"
#include "CubemapEntity.hpp"
#include "PlaneEntity.hpp"
#include "TreeEntity.hpp"
#include "LightEntity.hpp"
#include "RayEntity.hpp"
#include "PlayerEntity.hpp"
#include "GrassEntity.hpp"

class EntityManager
{
private:
	Renderer _renderer;
	
	unsigned int _nextTextureSlot = 0;		
	unsigned int _lastIndex = 0;
	unsigned int _grassCounter = 0;
	unsigned int _treeCounter = 0;
	unsigned int _lightCounter = 0;
	
	std::vector<Basemodel*> _models;
	std::vector<ObjmodelEntity*> _objEntitys;
	std::vector<PlaneEntity*> _planeEntitys;
	
	GrassEntity* _grassEntity;	
	TreeEntity* _treeEntity;	
	LightEntity* _lightEntity;	
	TerrainEntity* _terrainEntity;
	CubemapEntity* _cubemapEntity;
	RayEntity* _rayEntity;
	PlayerEntity* _playerEntity;	
	
public:	
	EntityManager()
	{
		
	}

	~EntityManager()
	{
		for (auto objs : _objEntitys)
			delete objs;

		for (auto planes : _planeEntitys)
			delete planes;
		
		delete _grassEntity;
		delete _treeEntity;		
		delete _lightEntity;
		delete _terrainEntity;
		delete _rayEntity;
		delete _cubemapEntity;
		delete _playerEntity;
	}
	
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	unsigned int addGrassEntity(const char* objFile, const char* texture)
	{
		if (_grassCounter == 0)
		{
			_grassEntity = new GrassEntity(objFile, texture, &_nextTextureSlot);
			_grassEntity->addGrass();
		}
		else
		{
			_grassEntity->addGrass();
		}

		_models.push_back((Basemodel*)(_grassEntity->_grassModels.at(_grassCounter)));
		unsigned int temp = _grassCounter;
		_grassCounter += 1;
		return temp;		
	}
	
	void translateGrassEntity(const unsigned int& grassID, const glm::vec3& tVec3) const
	{
		if (grassID >= 0 && grassID < _grassCounter)
			_grassEntity->translate(grassID, tVec3);
	}

	void rotateGrassEntity(const unsigned int& grassID, const float& angle, const glm::vec3& axis) const
	{
		if (grassID >= 0 && grassID < _grassCounter)
			_grassEntity->rotate(grassID, angle, axis);
	}

	void scaleGrassEntity(const unsigned int& grassID, const glm::vec3& scalar) const
	{
		if (grassID >= 0 && grassID < _grassCounter)
			_grassEntity->scale(grassID, scalar);
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	unsigned int addTreeEntity()
	{
		if (_treeCounter == 0)
		{
			_treeEntity = new TreeEntity(&_nextTextureSlot);
			_treeEntity->addTree();
		}
		else
		{
			_treeEntity->addTree();
		}

		_models.push_back((Basemodel*)(_treeEntity->_treeModels.at(_treeCounter)));
		_models.push_back((Basemodel*)(_treeEntity->_leafModels.at(_treeCounter)));
		unsigned int temp = _treeCounter;
		_treeCounter += 1;
		return temp;
	}
	
	void translateTreeEntity(const unsigned int& treeID, const glm::vec3& tVec3) const
	{
		if (treeID >= 0 && treeID < _treeCounter)
			_treeEntity->translate(treeID, tVec3);
	}

	void rotateTreeEntity(const unsigned int& treeID, const float& angle, const glm::vec3& axis) const
	{
		if (treeID >= 0 && treeID < _treeCounter)
			_treeEntity->rotate(treeID, angle, axis);
	}

	void scaleTreeEntity(const unsigned int& treeID, const glm::vec3& scalar) const
	{
		if (treeID >= 0 && treeID < _treeCounter)
			_treeEntity->scale(treeID, scalar);
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	unsigned int addLightEntity(const char* lanternOBJ, const char* lanternTexture, const char* lightbulbOBJ, const char* lightbulbTexture)
	{
		if(_lightCounter == 0)
		{
			_lightEntity = new LightEntity(lanternOBJ, lanternTexture, lightbulbOBJ, lightbulbTexture, &_nextTextureSlot);
			_lightEntity->addLight();
		}
		else
		{
			_lightEntity->addLight();
		}
		
		_models.push_back((Basemodel*)(_lightEntity->_lanternModels.at(_lightCounter)));
		_models.push_back((Basemodel*)(_lightEntity->_lightbulbModels.at(_lightCounter)));
		unsigned int temp = _lightCounter;
		_lightCounter += 1;
		return temp;
	}

	void translateLightEntity(const unsigned int& lightID, const glm::vec3& tVec3) const
	{
		if(lightID >= 0 && lightID < _lightCounter)
			_lightEntity->translate(lightID, tVec3);
	}
	
	void rotateLightEntity(const unsigned int& lightID, const float& angle, const glm::vec3& axis) const
	{
		if (lightID >= 0 && lightID < _lightCounter)
			_lightEntity->rotate(lightID, angle, axis);
	}

	void scaleLightEntity(const unsigned int& lightID, const glm::vec3& scalar) const
	{
		if (lightID >= 0 && lightID < _lightCounter)
			_lightEntity->scale(lightID, scalar);
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------	
	TerrainEntity* addTerrainEntity(const unsigned int& size, const unsigned int& tileSize, const char* heightmap, const char* terrainTexture, const char* pathwayTexture, const char* blendmap)
	{
		_terrainEntity = new TerrainEntity(size, tileSize, heightmap, terrainTexture, pathwayTexture, blendmap, &_nextTextureSlot);
		_models.push_back((Basemodel*)_terrainEntity->_groundModel);
		return _terrainEntity;
	}
	
	PlaneEntity* addPlaneEntity(const unsigned int& size, const unsigned int& tileSize, const char* texture)
	{
		PlaneEntity* plane = new PlaneEntity(size, tileSize, texture, &_nextTextureSlot);
		_planeEntitys.push_back(plane);
		_models.push_back((Basemodel*)plane->_planeModel);
		return plane;
	}

	ObjmodelEntity* addOBJEntity(const char* objFile, const char* texture)
	{
		ObjmodelEntity* obj = new ObjmodelEntity(objFile, texture, &_nextTextureSlot);
		_objEntitys.push_back(obj);
		_models.push_back((Basemodel*)obj->_standardmodel);
		return obj;
	}
	
	PlayerEntity* addPlayerEntity(DisplayManager* DM, AudioManager* AM, const glm::vec3& spawnPos)
	{
		_playerEntity = new PlayerEntity(DM, AM, spawnPos, _terrainEntity->_groundData, &_nextTextureSlot);
		_models.push_back((Basemodel*)_playerEntity->_playerModel);
		return _playerEntity;
	}	
	
	void addCubemapEntity()
	{
		_cubemapEntity = new CubemapEntity(&_nextTextureSlot);
		_models.push_back((Basemodel*)_cubemapEntity->_standardmodel);
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void createRay()
	{
		_rayEntity = new RayEntity();
		_models.push_back((Basemodel*)_rayEntity->_quaderModel);
	}
	
	void visualizeRay(const glm::vec3& camPosition, const glm::vec3& endPosition, const float& angle, bool& renderRay, const float& rayLength, const float& rayThickness) const
	{
		_rayEntity->updateRay(camPosition, endPosition, angle, renderRay, rayLength, rayThickness);
	}

	void colorPickedVertice(const glm::vec3& terrainEntry)
	{
		_terrainEntity->colorPickedVertice(terrainEntry, &_lastIndex);
	}

	void deleteLastColoredVertice()
	{
		_terrainEntity->deleteLastPickedVertice(&_lastIndex);
	}

	void raiseTerrain()
	{
		_terrainEntity->raise(&_lastIndex);
	}

	void sinkTerrain()
	{
		_terrainEntity->sink(&_lastIndex);
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void render()
	{
		_renderer.prepare();
		_renderer.render(_models);
	}
};