#pragma once

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "RawData.hpp"

class AssimpLoader : public RawData
{
private:
	const char* _filepath;

public:
	AssimpLoader(const char* filepath)
		: _filepath(filepath)
	{
		init();
	}

	void init()
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(_filepath, aiProcess_Triangulate);
		if (!scene) 
		{
			fprintf(stderr, importer.GetErrorString());
			getchar();
		}
		const aiMesh* mesh = scene->mMeshes[0]; // In this simple example code we always use the 1rst mesh (in OBJ files there is often only one anyway)
	
		// Fill vertices positions
		_vertices.reserve(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			aiVector3D pos = mesh->mVertices[i];
			_vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
		}

		// Fill vertices texture coordinates
		_texCoords.reserve(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
			_texCoords.push_back(glm::vec2(UVW.x, UVW.y));
		}

		// Fill vertices normals
		_normals.reserve(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			aiVector3D n = mesh->mNormals[i];
			_normals.push_back(glm::vec3(n.x, n.y, n.z));
		}

		// Fill face indices
		_indices.reserve(3 * mesh->mNumFaces);
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			// Assume the model has only triangles.
			_indices.emplace_back(glm::uvec3(mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2]));
		}
	
		setParameters();
	}

	void setParameters()
	{
		_verticeSize = _vertices.size() * sizeof(glm::vec3);
		_indiceSize = _indices.size() * sizeof(glm::uvec3);
		_texCoordSize = _texCoords.size() * sizeof(glm::vec2);
		_normalSize = _normals.size() * sizeof(glm::vec3);
		_verticesToRender = (GLsizei)_indices.size() * 3;
	}	
};