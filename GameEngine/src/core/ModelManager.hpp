#pragma once

#include "Model.hpp"
#include "Shader.hpp"
#include "GroundData.hpp"
#include "Texture.hpp"
#include "Filemanager.hpp"
#include "Groundmodel.hpp"

class ModelManager
{
private:
	Shader *_standard_shader, *_ground_shader;
	GroundData *_ground_data;
	Texture *_grass_tex;
	Basemodel *_ground;
	Model* _gr;
	Filemanager* _filemanager;

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
		_ground_data = new GroundData(128);
	}

	void initTextures()
	{
		_grass_tex = new Texture("res/textures/Dirt.jpg");
		_grass_tex->bind(0);
	}

	void createModels()
	{
		_ground = new Groundmodel(_ground_data, _ground_shader, 0, 1, 2);
	}

	void transformModels()
	{
		_ground->translate(glm::vec3(0.0f, -40.0f, 0.0f));
	}

	void addModelsToRenderer()
	{
		Models.push_back(_ground);
	}

	void debugVectors()
	{
		_filemanager = new Filemanager();		
		_filemanager->writeReadableToFile(_ground_data->_terrainColors, "res/data/terrainColors.data");
	}
};
