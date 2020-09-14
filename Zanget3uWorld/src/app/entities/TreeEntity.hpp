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
	std::vector<Standardmodel*> _treeModels;
	std::vector<Leafmodel*> _leafModels;
	friend class EntityManager;
	
public:
	TreeEntity(unsigned int* nextTextureSlot)
	{		
		//Create the data
		_treeData = new AssimpLoader("../res/obj/vegetation/TreeNaked.obj");
		_leafData = new AssimpLoader("../res/obj/vegetation/LeafsNaked.obj");
		
		//Create the textures
		_treeTexCount = *nextTextureSlot;
		_treeTex = new Texture("../res/textures/models/MapleTreeBark.jpg", _treeTexCount);
		(*nextTextureSlot)++;
		
		_leafTexCount = *nextTextureSlot;
		_leafTex = new Texture("../res/textures/models/MapleTreeLeaf.jpg", _leafTexCount);
		(*nextTextureSlot)++;
		
		_leafMaskCount = *nextTextureSlot;
		_leafMaskTex = new Texture("../res/textures/models/MapleTreeMask.jpg", _leafMaskCount);
		(*nextTextureSlot)++;
		
		//Create the shaders
		_treeShader = new Shader("../res/shader/zanget3uWorld/standard_vs.glsl", "../res/shader/zanget3uWorld/standard_fs.glsl");
		_leafShader = new Shader("../res/shader/zanget3uWorld/leaf_vs.glsl", "../res/shader/zanget3uWorld/leaf_fs.glsl");
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

		for (auto tree : _treeModels)
			delete tree;

		for (auto leaf : _leafModels)
			delete leaf;
	}

	void addTree()
	{
		Standardmodel* treeModel = new Standardmodel(_treeData, _treeShader, _treeTexCount);
		Leafmodel* leafModel = new Leafmodel(_leafData, _leafShader, _leafTexCount, _leafMaskCount);
		_treeModels.push_back(treeModel);
		_leafModels.push_back(leafModel);
	}
	
	void translate(const unsigned int& treeID, const glm::vec3& tVec3) const
	{
		_treeModels.at(treeID)->translate(tVec3);
		_leafModels.at(treeID)->translate(tVec3);
	}

	void rotate(const unsigned int& treeID, const float& angle, const glm::vec3& axis) const
	{
		_treeModels.at(treeID)->rotate(angle, axis);
		_leafModels.at(treeID)->rotate(angle, axis);
	}

	void scale(const unsigned int& treeID, const glm::vec3& scalar) const
	{
		_treeModels.at(treeID)->scale(scalar);
		_leafModels.at(treeID)->scale(scalar);
	}
};