#pragma once

#include "Shader.hpp"
#include "GroundData.hpp"
#include "Texture.hpp"
#include "Filemanager.hpp"
#include "Groundmodel.hpp"
#include "AssimpLoader.hpp"
#include "PlaneData.hpp"
#include "Leafmodel.hpp"
#include "Model.hpp"
#include "AudioManager.hpp"
#include "Cubemap.hpp"
#include "Lightmodel.hpp"
#include "LightPositions.hpp"

class ModelManager
{
private:
	Shader *_standard_shader, *_ground_shader, *_leaf_shader, *_cubeMap_shader, *_lightbulb_shader;
	GroundData *_ground_data;
	PlaneData *_water_data;
	AssimpLoader *_house_data, *_wood_data, *_axe_data, *_tree_data, *_leaf_data, *_grass_data, *_player_data, *_cubeMap_data, *_lantern_data, *_lightbulb_data;
	Texture *_grass_tex, *_dirt_tex, *_stone_tex, *_blendmap, *_house_tex, *_wood_tex, *_axe_tex, *_water_tex, *_tree_tex, *_leaf_tex, *_leafMask_tex, *_grassModel_tex, *_player_tex, *_lantern_tex, *_lightbulb_tex;
	Groundmodel *_ground;
	Model *_house, *_wood, *_axe, *_water, *_tree, *_grass, *_player, *_cubeMap, *_lantern;
	Lightmodel *_lightbulb;
	Leafmodel *_leaf;
	Filemanager *_filemanager;
	DisplayManager *_displayManager;
	AudioManager *_audioManager;
	Cubemap *_cubeMap_tex;

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
		createPlayer();
		debugVectors();
	}

	void initShader()
	{
		_standard_shader = new Shader("res/shader/standard_vs.glsl", "res/shader/standard_fs.glsl");
		_ground_shader = new Shader("res/shader/ground_vs.glsl", "res/shader/ground_fs.glsl");
		//_leaf_shader = new Shader("res/shader/leaf_vs.glsl", "res/shader/leaf_fs.glsl");
		_cubeMap_shader = new Shader("res/shader/cubemap_vs.glsl", "res/shader/cubemap_fs.glsl");
		_lightbulb_shader = new Shader("res/shader/lightbulb_vs.glsl", "res/shader/lightbulb_fs.glsl");
	}

	void createContainer()
	{
		_cubeMap_data = new AssimpLoader("res/obj/geometry/cube.obj");
		_ground_data = new GroundData(256, 2);
		//_water_data = new PlaneData(128, 1);
		_house_data = new AssimpLoader("res/obj/houses/Farmhouse.obj");
		/*_wood_data = new AssimpLoader("res/obj/vegetation/Wood.obj");
		_axe_data = new AssimpLoader("res/obj/tools/Axe.obj");
		_tree_data = new AssimpLoader("res/obj/vegetation/MapleTree.obj");
		_leaf_data = new AssimpLoader("res/obj/vegetation/MapleTreeLeaf.obj");
		_grass_data = new AssimpLoader("res/obj/vegetation/LowGrass.obj");
		_player_data = new AssimpLoader("res/obj/humans/Chibi.obj");*/	
		_lantern_data = new AssimpLoader("res/obj/lightsources/Parklight.obj");		
		_lightbulb_data = new AssimpLoader("res/obj/geometry/cylinder.obj");
	}

	void initTextures()
	{
		_grass_tex = new Texture("res/textures/Grass.jpg");
		_dirt_tex = new Texture("res/textures/Dirt.jpg");
		_blendmap = new Texture("res/maps/Blendmap_256_bold.jpg");
		_house_tex = new Texture("res/textures/models/Farmhouse.jpg");
		/*_wood_tex = new Texture("res/textures/models/Wood.jpg");
		_axe_tex = new Texture("res/textures/models/Axe.jpg");
		_water_tex = new Texture("res/textures/Water.jpg");
		_tree_tex = new Texture("res/textures/models/MapleTreeBark.jpg");
		_leaf_tex = new Texture("res/textures/models/MapleTreeLeaf.jpg");
		_leafMask_tex = new Texture("res/textures/models/MapleTreeMask.jpg");
		_grassModel_tex = new Texture("res/textures/models/Grass.jpg");
		_player_tex = new Texture("res/textures/models/Chibi.jpg");*/
		_lantern_tex = new Texture("res/textures/models/Metal_2_dark.jpg");
		_lightbulb_tex = new Texture("res/textures/models/White.jpg");

		_grass_tex->bind(0);
		_dirt_tex->bind(1);
		_blendmap->bind(2);
		_house_tex->bind(4);
		/*_wood_tex->bind(5);
		_axe_tex->bind(6);
		_water_tex->bind(7);
		_tree_tex->bind(8);
		_leaf_tex->bind(9);
		_leafMask_tex->bind(10);
		_grassModel_tex->bind(11);
		_player_tex->bind(12);*/
		_lantern_tex->bind(13);
		_lightbulb_tex->bind(14);

		std::vector<std::string> faces
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
		_cubeMap = new Model(_cubeMap_data, _cubeMap_shader, 3, glm::vec3(0,0,0), nullptr, true);		
		_ground = new Groundmodel(_ground_data, _ground_shader, 0, 1, 1, 2, _lightColor, _lightPositions);
		_house = new Model(_house_data, _standard_shader, 4, _lightColor, _lightPositions);
		/*_wood = new Model(_wood_data, _standard_shader, 5);
		_axe = new Model(_axe_data, _standard_shader, 6);
		_water = new Model(_water_data, _standard_shader, 7);
		_player = new Model(_player_data, _standard_shader, 12);*/
		_lantern = new Model(_lantern_data, _standard_shader, 13, _lightColor, _lightPositions);
		createLights();
	}

	void createLights()
	{
		for(int i = 0; i < numberOfPointlights; i++)
		{
			//Lightbulbs
			_lightbulb = new Lightmodel(_lightbulb_data, _lightbulb_shader, 14, _lightColor);
			_lightbulb->translate(_lightPositions[i]);
			_lightbulb->scale(_lightScale);
			Models.push_back(_lightbulb);

			//Lanterns
			/*_lantern = new Model(_lantern_data, _standard_shader, 13, _lightColor, _lightPositions);
			_lightbulb->translate(_lanternPositions[i]);
			Models.push_back(_lantern);*/
		}
	}

	void transformModels()
	{		
		//_ground->translate(glm::vec3(0.0f, -40.0f, 0.0f));
		_house->translate(glm::vec3(430, -5, 85));
		_house->scale(glm::vec3(1.75f, 1.75f, 1.75f));
		_house->rotate(45.0f, glm::vec3(0, 1, 0));
		_house->rotate(-7.0f, glm::vec3(1, 0, 0));
		/*_wood->translate(glm::vec3(425, -52, 115));
		_wood->scale(glm::vec3(2.5f, 2.5f, 2.5f));
		_axe->translate(glm::vec3(417, -39, 115));
		_axe->scale(glm::vec3(0.6f, 0.6f, 0.6f));
		_axe->rotate(-90.0f, glm::vec3(0, 1, 0));
		_axe->rotate(-90.0f, glm::vec3(0, 0, 1));
		_water->translate(glm::vec3(165, -51, 340));
		_water->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));*/
		_lantern->translate(_lanternPositions[0]);
		_lantern->scale(_lanternScale);
		//_lightbulb->translate(_lanternPositions[0]);
	}

	void addModelsToRenderer()
	{
		Models.push_back(_cubeMap);
		Models.push_back(_ground);
		Models.push_back(_house);
		Models.push_back(_lantern);
		/*Models.push_back(_wood);
		Models.push_back(_axe);
		Models.push_back(_water);
		Models.push_back(_player);
		Models.push_back(_lantern);*/		

		//for (int i = 0; i < 240; i++)
		//{
		//	//Koordinatenberechnung
		//	int grass_x = rand() % 256;
		//	int grass_z = rand() % 256;
		//	float grass_y = _ground_data->getHeightValue(grass_x, grass_z, 6.0) - 40.0f;
		//	grass_x *= 2;
		//	grass_z *= 2;

		//	int tree_x = rand() % 256;
		//	int tree_z = rand() % 256;
		//	float tree_y = _ground_data->getHeightValue(tree_x, tree_z, 6.0) - 40.0f;
		//	tree_x *= 2;
		//	tree_z *= 2;

		//	//Grass	
		//	if (grass_x > 160 && grass_x < 280 && grass_z > 200 && grass_z < 340)
		//	{
		//		//Kollision mit der Wasserflaeche
		//	}
		//	else if (grass_x > 380 && grass_x < 500 && grass_z > 40 && grass_z < 160)
		//	{
		//		//Kollision mit dem Haus
		//	}
		//	else
		//	{
		//		_grass = new Model(_grass_data, _standard_shader, 11);
		//		_grass->translate(glm::vec3(grass_x, grass_y, grass_z));
		//		int size = rand() % 30;
		//		_grass->scale(glm::vec3(size, size, size));
		//		Models.push_back(_grass);
		//	}

		//	//Tree
		//	if (i % 3 == 0)
		//	{
		//		if (tree_x > 160 && tree_x < 280 && tree_z > 200 && tree_z < 340)
		//		{
		//			//Kollision mit der Wasserflaeche
		//		}
		//		else if (tree_x > 380 && tree_x < 500 && tree_z > 40 && tree_z < 160)
		//		{
		//			//Kollision mit dem Haus
		//		}
		//		else
		//		{
		//			_tree = new Model(_tree_data, _standard_shader, 8);
		//			_leaf = new Leafmodel(_leaf_data, _leaf_shader, 9, 10, _displayManager);
		//			_tree->translate(glm::vec3(tree_x, tree_y, tree_z));
		//			_leaf->translate(glm::vec3(tree_x, tree_y, tree_z));
		//			Models.push_back(_tree);
		//			Models.push_back(_leaf);
		//		}
		//	}
		//}		
	}	

	void createPlayer()
	{
		//Create playerobject from model and translate it
		/*float x = 330 / 2;
		float z = 70 / 2;
		float y = _ground_data->getHeightValue(x, z, 6.0) - 39.8f;
		x *= 2;
		z *= 2;
		_displayManager->_player = new Player(_player, glm::vec3(x, y, z), _ground_data, _audioManager);*/
	}

	void debugVectors()
	{
		//_filemanager = new Filemanager();		
		//_filemanager->writeReadableToFile(_ground_data->_normals, "res/data/readable/normals.gldata");
		//_filemanager->writeReadableToFile(_ground_data->_vertices, "res/data/readable/vertices.gldata");
		//_filemanager->writeReadableToFile(_ground_data->_indices, "res/data/readable/indices.gldata");
	}
};
