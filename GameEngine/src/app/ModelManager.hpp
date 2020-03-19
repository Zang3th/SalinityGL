#pragma once

#include "Shader.hpp"
#include "GroundData.hpp"
#include "Texture.hpp"
#include "Filemanager.hpp"
#include "Groundmodel.hpp"
#include "AssimpLoader.hpp"
#include "PlaneData.hpp"
#include "Leafmodel.hpp"
#include "Standardmodel.hpp"
#include "AudioManager.hpp"
#include "Cubemap.hpp"
#include "Lightmodel.hpp"
#include "LightPositions.hpp"
#include "QuaderData.hpp"
#include "Primitivemodel.hpp"

unsigned int _last_index = 0;

class ModelManager
{
private:
	Shader *_standard_shader, *_ground_shader, *_leaf_shader, *_cubeMap_shader, *_lightbulb_shader, *_primitive_shader;
	GroundData *_ground_data;
	PlaneData *_water_data;
	AssimpLoader *_house_data, *_wood_data, *_axe_data, *_tree_data, *_leaf_data, *_grass_data, *_player_data, *_cubeMap_data, *_lantern_data, *_lightbulb_data;
	Texture *_grass_tex, *_dirt_tex, *_blendmap, *_house_tex, *_wood_tex, *_axe_tex, *_water_tex, *_tree_tex, *_leaf_tex, *_leafMask_tex, *_grassModel_tex, *_player_tex, *_lantern_tex, *_lightbulb_tex;
	Groundmodel *_ground;
	Standardmodel *_house, *_wood, *_axe, *_water, *_tree, *_grass, *_playerModel, *_cubeMap, *_lantern;
	Lightmodel *_lightbulb;
	Leafmodel *_leaf;
	Filemanager *_filemanager;
	DisplayManager *_displayManager;
	AudioManager *_audioManager;
	Cubemap *_cubeMap_tex;
	QuaderData *_quaderData;
	Primitivemodel *_quader = nullptr;	
	
public:
	std::vector<Basemodel*> Models;	
	
	ModelManager(DisplayManager* displayManager, AudioManager* audioManager)
		: _displayManager(displayManager), _audioManager(audioManager)
	{
		initShader();
		createContainer();
		initTextures();
		createModels();
		transformModels();
		addModelsToRenderer();
		//debugVectors();
	}

	void initShader()
	{
		_standard_shader = new Shader("res/shader/standard_vs.glsl", "res/shader/standard_fs.glsl");
		_ground_shader = new Shader("res/shader/ground_vs.glsl", "res/shader/ground_fs.glsl");
		_cubeMap_shader = new Shader("res/shader/cubemap_vs.glsl", "res/shader/cubemap_fs.glsl");
		_leaf_shader = new Shader("res/shader/leaf_vs.glsl", "res/shader/leaf_fs.glsl");		
		_lightbulb_shader = new Shader("res/shader/lightbulb_vs.glsl", "res/shader/lightbulb_fs.glsl");
		_primitive_shader = new Shader("res/shader/primitive_vs.glsl", "res/shader/primitive_fs.glsl");
	}

	void createContainer()
	{
		_cubeMap_data = new AssimpLoader("res/obj/geometry/cube.obj");
		_ground_data = new GroundData(512, 1, "res/maps/Heightmap_512.bmp");
		_water_data = new PlaneData(160, 1);
		_house_data = new AssimpLoader("res/obj/houses/Farmhouse.obj");
		_wood_data = new AssimpLoader("res/obj/vegetation/Wood.obj");
		_axe_data = new AssimpLoader("res/obj/tools/Axe.obj");
		_tree_data = new AssimpLoader("res/obj/vegetation/TreeNaked.obj");
		_leaf_data = new AssimpLoader("res/obj/vegetation/LeafsNaked.obj");
		_grass_data = new AssimpLoader("res/obj/vegetation/LowGrass.obj");
		_player_data = new AssimpLoader("res/obj/humans/Chibi.obj");	
		_lantern_data = new AssimpLoader("res/obj/lightsources/Parklight.obj");		
		_lightbulb_data = new AssimpLoader("res/obj/geometry/cylinder.obj");
		_quaderData = new QuaderData();
	}

	void initTextures()
	{
		_grass_tex = new Texture("res/textures/Grass.jpg");
		_dirt_tex = new Texture("res/textures/Dirt.jpg");
		_blendmap = new Texture("res/maps/Blendmap_512.jpg");
		_house_tex = new Texture("res/textures/models/Farmhouse.jpg");
		_wood_tex = new Texture("res/textures/models/Wood.jpg");
		_axe_tex = new Texture("res/textures/models/Axe.jpg");
		_water_tex = new Texture("res/textures/Water.jpg");
		_tree_tex = new Texture("res/textures/models/MapleTreeBark.jpg");
		_leaf_tex = new Texture("res/textures/models/MapleTreeLeaf.jpg");
		_leafMask_tex = new Texture("res/textures/models/MapleTreeMask.jpg");
		_grassModel_tex = new Texture("res/textures/models/Grass.jpg");
		_player_tex = new Texture("res/textures/models/Chibi.jpg");
		_lantern_tex = new Texture("res/textures/models/Metal_2_dark.jpg");
		_lightbulb_tex = new Texture("res/textures/models/White.jpg");

		_grass_tex->bind(0);
		_dirt_tex->bind(1);
		_blendmap->bind(2);
		_house_tex->bind(4);
		_wood_tex->bind(5);
		_axe_tex->bind(6);
		_water_tex->bind(7);
		_tree_tex->bind(8);
		_leaf_tex->bind(9);
		_leafMask_tex->bind(10);
		_grassModel_tex->bind(11);
		_player_tex->bind(12);
		_lantern_tex->bind(13);
		_lightbulb_tex->bind(14);

		std::vector<const char*> faces
		{
				"res/textures/cubeMap/xp.jpg",
				"res/textures/cubeMap/xn.jpg",
				"res/textures/cubeMap/yp.jpg",
				"res/textures/cubeMap/yn.jpg",
				"res/textures/cubeMap/zp.jpg",
				"res/textures/cubeMap/zn.jpg"
		};

		_cubeMap_tex = new Cubemap(faces);
		_cubeMap_tex->bind(3);
	}

	void createModels()
	{
		_cubeMap = new Standardmodel(_cubeMap_data, _cubeMap_shader, 3, true);
		_ground = new Groundmodel(_ground_data, _ground_shader, 0, 1, 1, 2);
		_house = new Standardmodel(_house_data, _standard_shader, 4);
		_wood = new Standardmodel(_wood_data, _standard_shader, 5);
		_axe = new Standardmodel(_axe_data, _standard_shader, 6);
		_water = new Standardmodel(_water_data, _standard_shader, 7);
		_playerModel = new Standardmodel(_player_data, _standard_shader, 12);
		_lantern = new Standardmodel(_lantern_data, _standard_shader, 13);
		_quader = new Primitivemodel(_quaderData, _primitive_shader);
	}	

	void transformModels() const
	{		
		_house->translate(glm::vec3(440, -1.5, 140));
		_house->scale(glm::vec3(2.25f, 2.25f, 2.25f));		
		_house->rotate(170.0f, glm::vec3(0, 1, 0));
		_house->rotate(-5.0f, glm::vec3(1, 0, 0));
		_house->rotate(-3.0f, glm::vec3(0, 0, 1));
		
		_wood->translate(glm::vec3(412, -3.0, 116));
		_wood->scale(glm::vec3(2.5f, 2.5f, 2.5f));
				
		_axe->translate(glm::vec3(408, 3.5, 114));
		_axe->scale(glm::vec3(0.4f, 0.4f, 0.4f));
		_axe->rotate(120.0f, glm::vec3(0, -1, 0));
		_axe->rotate(120.0f, glm::vec3(0, 0, -1));

		_water->translate(glm::vec3(190, -4.5, 480));
		_water->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void addModelsToRenderer()
	{
		Models.push_back(_cubeMap);
		Models.push_back(_ground);
		Models.push_back(_house);
		Models.push_back(_wood);
		Models.push_back(_axe);
		Models.push_back(_water);
		Models.push_back(_playerModel);			
		
		createLights();
		createPlayer();
		createVegetation();

		Models.push_back(_quader);
	}

	void createLights()
	{
		for (int i = 0; i < numberOfPointlights; i++)
		{
			//Lightbulbs
			_lightbulb = new Lightmodel(_lightbulb_data, _lightbulb_shader, 14, _lightColor);			
			_lightbulb->translate(_lightPositions[i]);
			_lightbulb->scale(_lightScale);
			Models.push_back(_lightbulb);

			//Lanterns
			_lantern = new Standardmodel(_lantern_data, _standard_shader, 13);			
			_lantern->translate(_lanternPositions[i]);
			_lantern->scale(_lanternScale);
			Models.push_back(_lantern);
		}
	}

	void createPlayer() const
	{
		//Create playerobject from model and translate it
		float x = 330.0f;
		float z = 70.0f;
		float y = _ground_data->getHeightValueUnbuffered(x, z);
		_displayManager->_player = new Player(_playerModel, glm::vec3(x, y, z), _ground_data, _audioManager);
	}

	void createVegetation()
	{
		for (int i = 0; i < 90; i++)
		{
			//Koordinatenberechnung
			int grass_x = rand() % 512;
			int grass_z = rand() % 512;
			float grass_y = _ground_data->getHeightValueUnbuffered(grass_x, grass_z);

			int tree_x = rand() % 512;
			int tree_z = rand() % 512;
			float tree_y = _ground_data->getHeightValueUnbuffered(tree_x, tree_z);

			//Grass	
			if (grass_x > 160 && grass_x < 280 && grass_z > 200 && grass_z < 340)
			{
				//Kollision mit der Wasserflaeche
			}
			else if (grass_x > 380 && grass_x < 500 && grass_z > 40 && grass_z < 160)
			{
				//Kollision mit dem Haus
			}
			else
			{
				_grass = new Standardmodel(_grass_data, _standard_shader, 11);
				int size = rand() % 30;
				_grass->translate(glm::vec3(grass_x, grass_y, grass_z));
				_grass->scale(glm::vec3(size, size, size));				
				Models.push_back(_grass);
			}

			//Tree
			if (i % 3 == 0)
			{
				if (tree_x > 160 && tree_x < 280 && tree_z > 200 && tree_z < 340)
				{
					//Kollision mit der Wasserflaeche
				}
				else if (tree_x > 380 && tree_x < 500 && tree_z > 40 && tree_z < 160)
				{
					//Kollision mit dem Haus
				}
				else
				{
					_tree = new Standardmodel(_tree_data, _standard_shader, 8);
					_leaf = new Leafmodel(_leaf_data, _leaf_shader, 9, 10);
					_tree->translate(glm::vec3(tree_x, tree_y, tree_z));
					_leaf->translate(glm::vec3(tree_x, tree_y, tree_z));
					Models.push_back(_tree);
					Models.push_back(_leaf);
				}
			}
		}	
	}

	void createRay(const glm::vec3& camPosition, const glm::vec3& endPosition, const float& angle, bool& renderRay, const float& rayLength, const float& rayThickness) const
	{	
		_quaderData->updatePosition(camPosition, endPosition, angle, rayLength, rayThickness);
		_quader->updateData(_quaderData);		
		
		if (renderRay == false)
			_quader->renderModel = false;
		else
			_quader->renderModel = true;		
	}	

	void colorPickedVertices(const glm::vec3& terrainEntry) const
	{
		//Delete color of the last picked vertice
		_ground->getRawData()->_isPicked.at(_last_index) = glm::vec3(1.0, 1.0, 1.0);

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
		const unsigned int index = _ground->getGroundData()->_twoDimArray[x][z];
		_ground->getRawData()->_isPicked.at(index) = glm::vec3(255, 0, 0);
		_ground->updateColorOfPickedVertices();
		
		_last_index = index;			
	}

	void deleteLastColoredVertice() const
	{
		_ground->getRawData()->_isPicked.at(_last_index) = glm::vec3(1.0, 1.0, 1.0);
		_ground->updateColorOfPickedVertices();
	}
	
	void raiseTerrain() const
	{
		_ground->getRawData()->_vertices.at(_last_index).y += 0.05;
		_ground->updateHeightOfPickedVertices();
	}
	
	void sinkTerrain() const
	{
		_ground->getRawData()->_vertices.at(_last_index).y -= 0.05;
		_ground->updateHeightOfPickedVertices();
	}
	
	void debugVectors()
	{
		_filemanager = new Filemanager();		
		_filemanager->writeReadableToFile(_ground_data->_normals, "res/data/readable/normals.gldata");
		_filemanager->writeReadableToFile(_ground_data->_vertices, "res/data/readable/vertices.gldata");
		_filemanager->writeReadableToFile(_ground_data->_indices, "res/data/readable/indices.gldata");
		_filemanager->writeReadableToFile(_quaderData->_vertices, "res/data/readable/quaderVertices.gldata");
		_filemanager->writeReadableToFile(_quaderData->_indices, "res/data/readable/quaderIndices.gldata");
	}

	GroundData* getTerrain() const
	{
		return _ground_data;
	}
};