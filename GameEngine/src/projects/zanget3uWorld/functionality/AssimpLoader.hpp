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
			_normals.emplace_back(0, 0, 0);
		}

		// Fill vertices texture coordinates
		_texCoords.reserve(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
			_texCoords.push_back(glm::vec2(UVW.x, UVW.y));
		}		

		// Fill face indices
		_indices.reserve(3 * mesh->mNumFaces);
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			// Assume the model has only triangles.
			_indices.emplace_back(glm::uvec3(mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2]));
			
		}

		// Fill vertices normals
		if(mesh->HasNormals())
		{
			_normals.clear();
			_normals.reserve(mesh->mNumVertices);
			for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
				aiVector3D n = mesh->mNormals[i];
				_normals.push_back(glm::vec3(n.x, n.y, n.z));
			}
		}
		else
		{
			spdlog::warn(".obj File had no normals: {}", _filepath);
			calculate_normals_per_vertex();
		}
		
		setParameters();
	}

	void calculate_normals_per_vertex()
	{
		for (int i = 0; i < _indices.size(); i++)
		{
			int index0 = _indices.at(i).x;
			int index1 = _indices.at(i).y;
			int index2 = _indices.at(i).z;

			glm::vec3 point0 = _vertices.at(index0);
			glm::vec3 point1 = _vertices.at(index1);
			glm::vec3 point2 = _vertices.at(index2);

			float u0 = point1.x - point0.x;
			float u1 = point1.y - point0.y;
			float u2 = point1.z - point0.z;
			glm::vec3 U(u0, u1, u2);

			float v0 = point2.x - point0.x;
			float v1 = point2.y - point0.y;
			float v2 = point2.z - point0.z;
			glm::vec3 V(v0, v1, v2);

			glm::vec3 p = glm::cross(U, V);

			_normals.at(index0) += p;
			_normals.at(index1) += p;
			_normals.at(index2) += p;
		}

		for (int j = 0; j < _normals.size(); j++)
		{
			glm::vec3 normal = _normals.at(j);
			normal = glm::normalize(normal);
			_normals.at(j) = normal;
		}
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