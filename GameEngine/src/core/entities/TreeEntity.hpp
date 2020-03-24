#pragma once

#include "AssimpLoader.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Standardmodel.hpp"
#include "Leafmodel.hpp"

class TreeEntity
{
private:
	AssimpLoader* _treeData,* _leafData;
	Texture* _treeTex, * _leafTex,* _leafMaskTex;
	unsigned int _treeTexCount, _leafTexCount, _leafMaskCount;
	Shader* _treeShader,* _leafShader;
	Standardmodel* _treeModel;
	Leafmodel* _leafModel;
	friend class EntityManager;
	
public:
	TreeEntity(unsigned int* nextTextureSlot)
	{		
		//Create the data
		_treeData = new AssimpLoader("res/obj/vegetation/TreeNaked.obj");
		_leafData = new AssimpLoader("res/obj/vegetation/LeafsNaked.obj");
		
		//Create the textures
		_treeTexCount = *nextTextureSlot;
		_treeTex = new Texture("res/textures/models/MapleTreeBark.jpg", _treeTexCount);
		(*nextTextureSlot)++;
		
		_leafTexCount = *nextTextureSlot;
		_leafTex = new Texture("res/textures/models/MapleTreeLeaf.jpg", _leafTexCount);
		(*nextTextureSlot)++;
		
		_leafMaskCount = *nextTextureSlot;
		_leafMaskTex = new Texture("res/textures/models/MapleTreeMask.jpg", _leafMaskCount);
		(*nextTextureSlot)++;
		
		//Create the shaders
		_treeShader = new Shader("res/shader/standard_vs.glsl", "res/shader/standard_fs.glsl");
		_leafShader = new Shader("res/shader/leaf_vs.glsl", "res/shader/leaf_fs.glsl");
		
		//Combine everything to the model
		_treeModel = new Standardmodel(_treeData, _treeShader, _treeTexCount);
		_leafModel = new Leafmodel(_leafData, _leafShader, _leafTexCount, _leafMaskCount);
	}

	~TreeEntity()
	{
		delete _treeData;
		delete _leafData;
		delete _treeTex;
		delete _leafTex;
		delete _leafMaskTex;
		delete _treeShader;
		delete _leafShader;
		delete _treeModel;
		delete _leafModel;
	}
	
	void translate(const glm::vec3& tVec3) const
	{
		_treeModel->translate(tVec3);
		_leafModel->translate(tVec3);
	}

	void rotate(const float& angle, const glm::vec3& axis) const
	{
		_treeModel->rotate(angle, axis);
		_leafModel->rotate(angle, axis);
	}

	void scale(const glm::vec3& scalar) const
	{
		_treeModel->scale(scalar);
		_leafModel->scale(scalar);
	}
};