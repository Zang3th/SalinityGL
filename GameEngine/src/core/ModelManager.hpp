#pragma once

#include "Model.hpp"
#include "Shader.hpp"
#include "GroundData.hpp"
#include "Texture.hpp"
#include "Filemanager.hpp"
#include "Groundmodel.hpp"
#include "AssimpLoader.hpp"
#include "PlaneData.hpp"

class ModelManager
{
private:
	Shader *_standard_shader, *_ground_shader;
	GroundData *_ground_data;
	PlaneData* _water_data;
	AssimpLoader *_house_data, *_wood_data, *_axe_data;
	Texture *_grass_tex, *_dirt_tex, *_stone_tex, *_blendmap, *_house_tex, * _wood_tex, *_axe_tex, *_water_tex;
	Basemodel *_ground;
	Model *_house, *_wood, *_axe, * _water;
	Filemanager *_filemanager;

public:
	std::vector<Basemodel*> Models;

	ModelManager()
	{
		initShader();
		initData();
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
	}

	void initData()
	{
		_ground_data = new GroundData(256, 2);
		_house_data = new AssimpLoader("res/obj/houses/Farmhouse.obj");
		_wood_data = new AssimpLoader("res/obj/vegetation/Wood.obj");
		_axe_data = new AssimpLoader("res/obj/tools/Axe.obj");
		_water_data = new PlaneData(128, 1);
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
		_grass_tex->bind(0);
		_dirt_tex->bind(1);
		_stone_tex->bind(2);
		_blendmap->bind(3);
		_house_tex->bind(4);
		_wood_tex->bind(5);
		_axe_tex->bind(6);
		_water_tex->bind(7);
	}

	void createModels()
	{
		_ground = new Groundmodel(_ground_data, _ground_shader, 0, 1, 2, 3);
		_house = new Model(_house_data, _standard_shader, 4);
		_wood = new Model(_wood_data, _standard_shader, 5);
		_axe = new Model(_axe_data, _standard_shader, 6);
		_water = new Model(_water_data, _standard_shader, 7);
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
	}

	void debugVectors()
	{
		_filemanager = new Filemanager();		
	}
};