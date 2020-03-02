#pragma once

#include "RawData.hpp"

class QuaderData : public RawData
{
private:
	glm::vec3 _startPosition, _endPosition;

public:
	QuaderData(const glm::vec3& startPosition, const glm::vec3& endPosition)
		: _startPosition(startPosition), _endPosition(endPosition)
	{
		init();
	}

	void init()
	{
		/*float vert[]
		{*/
			//-0.5f,  0.5f, -0.5f, // top left
			//-0.5f, -0.5f, -0.5f, // bottom left
			// 0.5f, -0.5f, -0.5f, // bottom right
			// 0.5f,  0.5f, -0.5f, // top right 

			/* 0.0f, 0.0f, 0.0f,
			 0.0f, 0.0f, 1.0f,
			 0.0f, 1.0f, 1.0f,
			 0.0f, 1.0f, 0.0f*/

			/*-0.5f,  0.5f, 0.5f, 
			-0.5f, -0.5f, 0.5f, 
			 0.5f, -0.5f, 0.5f, 
			 0.5f,  0.5f, 0.5f, 

			 0.5f,  0.5f, -0.5f, 
			 0.5f, -0.5f, -0.5f, 
			 0.5f, -0.5f,  0.5f, 
			 0.5f,  0.5f,  0.5f, 

			 -0.5f,  0.5f, -0.5f,
			 -0.5f, -0.5f, -0.5f,
			 -0.5f, -0.5f,  0.5f,
			 -0.5f,  0.5f,  0.5f,

			 -0.5f, 0.5f,  0.5f, 
			 -0.5f, 0.5f, -0.5f, 
			  0.5f, 0.5f, -0.5f, 
			  0.5f, 0.5f,  0.5f, */

			/* -0.5f, -0.5f,  0.5f,
			 -0.5f, -0.5f, -0.5f,
			  0.5f, -0.5f, -0.5f,
			  0.5f, -0.5f,  0.5f*/
		/*};

		std::vector<float> floats = std::vector<float>(vert, vert + sizeof(vert) / sizeof(vert[0]));

		for(int i = 0; i < floats.size(); i += 3)
		{
			_vertices.emplace_back(glm::vec3(floats.at(i), floats.at(i + 1), floats.at(i + 2)));
		}

		unsigned int ind[] =
		{
			0, 1, 2,
			0, 2, 3*/

			//0, 1, 3,
			//3, 1, 2,
			/*
			4, 5, 7,
			7, 5, 6,
			8, 9, 11,
			11, 9, 10,
			12, 13, 15,
			15, 13, 14,
			16, 17, 19,
			19, 17, 18,
			20, 21, 23,
			23, 21, 22*/
		/*};
		
		std::vector<unsigned int> uInts = std::vector<unsigned int>(ind, ind + sizeof(ind) / sizeof(ind[0]));

		for (int i = 0; i < uInts.size(); i += 3)
		{
			_indices.emplace_back(glm::uvec3(uInts.at(i), uInts.at(i + 1), uInts.at(i + 2)));
		}
		*/

		//for(int i = 0; i < 1; i++)
		//{
			//Front Face
			_vertices.emplace_back(glm::vec3(_startPosition.x, _startPosition.y, _startPosition.z));
			_vertices.emplace_back(glm::vec3(_startPosition.x, _startPosition.y, _startPosition.z + 1));
			_vertices.emplace_back(glm::vec3(_startPosition.x, _startPosition.y + 1, _startPosition.z + 1));
			_vertices.emplace_back(glm::vec3(_startPosition.x, _startPosition.y + 1, _startPosition.z));
			_indices.emplace_back(glm::uvec3(0, 1, 2));
			_indices.emplace_back(glm::uvec3(0, 2, 3));

			glm::vec3 calcEndPos = glm::vec3(_startPosition.x + _endPosition.x * 100, _startPosition.y + _endPosition.y * 100, _startPosition.z + _endPosition.z * 100);
			//glm::vec3 calcEndPos = glm::vec3(_endPosition.x * 100, _startPosition.y, _endPosition.z * 100);
			//glm::vec3 calcEndPos = _endPosition;
			//glm::vec3 calcEndPos = glm::vec3(_startPosition.x + 50, _startPosition.y, _startPosition.z);

			//Back Face
			_vertices.emplace_back(glm::vec3(calcEndPos.x, calcEndPos.y, calcEndPos.z));
			_vertices.emplace_back(glm::vec3(calcEndPos.x, calcEndPos.y, calcEndPos.z + 1));
			_vertices.emplace_back(glm::vec3(calcEndPos.x, calcEndPos.y + 1, calcEndPos.z + 1));
			_vertices.emplace_back(glm::vec3(calcEndPos.x, calcEndPos.y + 1, calcEndPos.z));
			_indices.emplace_back(glm::uvec3(4, 5, 6));
			_indices.emplace_back(glm::uvec3(4, 6, 7));

			//Bottom
			//_indices.emplace_back(glm::uvec3(0, 1, 5));
			//_indices.emplace_back(glm::uvec3(0, 5, 4));

			////Right
			//_indices.emplace_back(glm::uvec3(1, 5, 6));
			//_indices.emplace_back(glm::uvec3(1, 6, 2));

			////Top
			//_indices.emplace_back(glm::uvec3(3, 2, 6));
			//_indices.emplace_back(glm::uvec3(3, 6, 7));

			////Left
			//_indices.emplace_back(glm::uvec3(0, 4, 7));
			//_indices.emplace_back(glm::uvec3(0, 7, 3));
		//}

		setParameters();
	}

	void setParameters()
	{
		_verticeSize = _vertices.size() * sizeof(glm::vec3);
		_indiceSize = _indices.size() * sizeof(glm::uvec3);
		_verticesToRender = (GLsizei)_indices.size() * 3;
	}
};