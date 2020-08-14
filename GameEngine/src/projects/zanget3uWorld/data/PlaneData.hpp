#pragma once

#include "RawData.hpp"

class PlaneData : public RawData
{
private:
	unsigned int _size, _fieldmultiplier;

public:
	PlaneData(int size, int fieldmultiplier)
		: _size(size), _fieldmultiplier(fieldmultiplier)
	{
		init();
	}

	void init()
	{
		for (int j = 0; j <= _size; ++j)
		{
			for (int i = 0; i <= _size; ++i)
			{
				float x = (float)i / ((float)_size / (_size * _fieldmultiplier));
				float y = 0;
				float z = (float)j / ((float)_size / (_size * _fieldmultiplier));

				_vertices.emplace_back(glm::vec3(x, y, z));
				_texCoords.emplace_back(glm::vec2(x, z));				
				_normals.emplace_back(glm::vec3(0, -1, 0));

				if ((j != _size) && (i != _size))
				{
					const int row1 = j * (_size + 1);
					const int row2 = (j + 1) * (_size + 1);

					// triangle 1
					_indices.emplace_back(glm::uvec3(row1 + i, row1 + i + 1, row2 + i + 1));

					// triangle 2
					_indices.emplace_back(glm::uvec3(row1 + i, row2 + i + 1, row2 + i));
				}
			}
		}
		calculate_normals_per_vertex();
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