#pragma once

#include "Model.hpp"
#include "Shader.hpp"
#include "GroundData.hpp"
#include "Texture.hpp"
#include "Filemanager.hpp"
#include "Groundmodel.hpp"
#include "AssimpLoader.hpp"
#include "PlaneData.hpp"
#include "Leafmodel.hpp"

class ModelManager
{
private:
	Shader *_standard_shader, *_ground_shader, *_leaf_shader;
	GroundData *_ground_data;
	PlaneData* _water_data;
	AssimpLoader* _house_data, * _wood_data, * _axe_data, 
	* _tree_data, * _leaf_data, * _grass_data, * _player_data;
	Texture *_grass_tex, *_dirt_tex, *_stone_tex, 
	*_blendmap, *_house_tex, *_wood_tex, *_axe_tex, 
	*_water_tex, *_tree_tex, *_leaf_tex, *_leafMask_tex,
	*_grassModel_tex, *_player_tex;
	Basemodel *_ground;
	Model* _house, * _wood, * _axe, * _water, * _tree, * _grass, * _player;
	Leafmodel *_leaf;
	Filemanager *_filemanager;
	DisplayManager* _displayManager;

public:
	std::vector<Basemodel*> Models;

	ModelManager(DisplayManager* displayManager)
		: _displayManager(displayManager)
	{
		initShader();
		initData();
		initTextures();
		createModels();
		transformModels();
		addModelsToRenderer();
		createPlayer();
		//debugVectors();
	}

	void initShader()
	{
		_standard_shader = new Shader("res/shader/standard_vs.glsl", "res/shader/standard_fs.glsl");
		_ground_shader = new Shader("res/shader/ground_vs.glsl", "res/shader/ground_fs.glsl");
		_leaf_shader = new Shader("res/shader/leaf_vs.glsl", "res/shader/leaf_fs.glsl");
	}

	void initData()
	{
		_ground_data = new GroundData(256, 2);
		_house_data = new AssimpLoader("res/obj/houses/Farmhouse.obj");
		_wood_data = new AssimpLoader("res/obj/vegetation/Wood.obj");
		_axe_data = new AssimpLoader("res/obj/tools/Axe.obj");
		_water_data = new PlaneData(128, 1);
		_tree_data = new AssimpLoader("res/obj/vegetation/MapleTree.obj");
		_leaf_data = new AssimpLoader("res/obj/vegetation/MapleTreeLeaf.obj");
		_grass_data = new AssimpLoader("res/obj/vegetation/LowGrass.obj");
		_player_data = new AssimpLoader("res/obj/humans/Chibi.obj");
	}

	void initTextures()
	{
		_grass_tex = new Texture("res/textures/Grass.jpg");
		_dirt_tex = new Texture("res/textures/Dirt.jpg");
		_stone_tex = new Texture("res/textures/Stone_2.jpg");
		_blendmap = new Texture("res/maps/blendmap_256_bold.jpg");
		_house_tex = new Texture("res/textures/models/Farmhouse.jpg");
		_wood_tex = new Texture("res/textures/models/Wood.jpg");
		_axe_tex = new Texture("res/textures/models/Axe.jpg");
		_water_tex = new Texture("res/textures/Water.jpg");
		_tree_tex = new Texture("res/textures/models/MapleTreeBark.jpg");
		_leaf_tex = new Texture("res/textures/models/MapleTreeLeaf.jpg");
		_leafMask_tex = new Texture("res/textures/models/MapleTreeMask.jpg");
		_grassModel_tex = new Texture("res/textures/models/Grass.jpg");
		_player_tex = new Texture("res/textures/models/Chibi.jpg");
		_grass_tex->bind(0);
		_dirt_tex->bind(1);
		_stone_tex->bind(2);
		_blendmap->bind(3);
		_house_tex->bind(4);
		_wood_tex->bind(5);
		_axe_tex->bind(6);
		_water_tex->bind(7);
		_tree_tex->bind(8);
		_leaf_tex->bind(9);
		_leafMask_tex->bind(10);
		_grassModel_tex->bind(11);
		_player_tex->bind(12);
	}

	void createModels()
	{
		_ground = new Groundmodel(_ground_data, _ground_shader, 0, 1, 2, 3);
		_house = new Model(_house_data, _standard_shader, 4);
		_wood = new Model(_wood_data, _standard_shader, 5);
		_axe = new Model(_axe_data, _standard_shader, 6);
		_water = new Model(_water_data, _standard_shader, 7);
		_player = new Model(_player_data, _standard_shader, 12);
	}

	void transformModels()
	{
		_ground->translate(glm::vec3(0.0f, -40.0f, 0.0f));
		_house->translate(glm::vec3(430, -45, 85));
		_house->scale(glm::vec3(1.75f, 1.75f, 1.75f));
		_house->rotate(45.0f, glm::vec3(0, 1, 0));
		_house->rotate(-7.0f, glm::vec3(1, 0, 0));
		_wood->translate(glm::vec3(425, -52, 115));
		_wood->scale(glm::vec3(2.5f, 2.5f, 2.5f));
		_axe->translate(glm::vec3(417, -39, 115));
		_axe->scale(glm::vec3(0.6f, 0.6f, 0.6f));
		_axe->rotate(-90.0f, glm::vec3(0, 1, 0));
		_axe->rotate(-90.0f, glm::vec3(0, 0, 1));
		_water->translate(glm::vec3(165, -51, 340));
		_water->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void addModelsToRenderer()
	{
		Models.push_back(_ground);
		Models.push_back(_house);
		Models.push_back(_wood);
		Models.push_back(_axe);
		Models.push_back(_water);
		Models.push_back(_player);

		for (int i = 0; i < 240; i++)
		{
			//Koordinatenberechnung
			int grass_x = rand() % 256;
			int grass_z = rand() % 256;
			float grass_y = _ground_data->getHeightValue(grass_x, grass_z, 6.0) - 40.0f;
			grass_x *= 2;
			grass_z *= 2;

			int tree_x = rand() % 256;
			int tree_z = rand() % 256;
			float tree_y = _ground_data->getHeightValue(tree_x, tree_z, 6.0) - 40.0f;
			tree_x *= 2;
			tree_z *= 2;

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
				_grass = new Model(_grass_data, _standard_shader, 11);
				_grass->translate(glm::vec3(grass_x, grass_y, grass_z));
				int size = rand() % 30;
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
					_tree = new Model(_tree_data, _standard_shader, 8);
					_leaf = new Leafmodel(_leaf_data, _leaf_shader, 9, 10, _displayManager);
					_tree->translate(glm::vec3(tree_x, tree_y, tree_z));
					_leaf->translate(glm::vec3(tree_x, tree_y, tree_z));
					Models.push_back(_tree);
					Models.push_back(_leaf);
				}
			}
		}		
	}	

	void createPlayer()
	{
		//Create playerobject from model and translate it
		float x = 330 / 2;
		float z = 70 / 2;
		float y = _ground_data->getHeightValue(x, z, 6.0) - 39.8f;
		x *= 2;
		z *= 2;
		_displayManager->_player = new Player(_player, glm::vec3(x, y, z), _ground_data);
	}

	void debugVectors()
	{
		_filemanager = new Filemanager();		
	}
};