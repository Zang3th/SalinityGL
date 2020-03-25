#pragma once

#include "RawData.hpp"
#include "Heightmap.hpp"

struct Point
{
	float _x;
	float _y;
	float _z;

	Point(float x, float y, float z)
		: _x(x), _y(y), _z(z)
	{}
};

struct Triangle
{
	Point _P0;
	Point _P1;
	Point _P2;

	Triangle(Point P0, Point P1, Point P2)
		: _P0(P0), _P1(P1), _P2(P2)
	{}
};

class GroundData : public RawData
{
private:
	unsigned int _size, _fieldmultiplier;
	const char* _heightmap_filepath;
	Heightmap* _heightmap = nullptr;
	
public:
	unsigned int _index = 0;
	unsigned int _twoDimArray[512][512];
	
	GroundData(const unsigned int& size, const unsigned int& fieldmultiplier, const char* heightmap_filepath)
		: _size(size), _fieldmultiplier(fieldmultiplier), _heightmap_filepath(heightmap_filepath)
	{
		init();
	}

	~GroundData()
	{
		delete _heightmap;
	}
	
	void init()
	{		
		_heightmap = new Heightmap(_heightmap_filepath, 0);
		
		for (int j = 0; j <= _size; ++j)
		{
			for (int i = 0; i <= _size; ++i)
			{
				float x = (float)i / ((float)_size / (_size * _fieldmultiplier));
				float y = getHeightValueBuffered(i, j);
				float z = (float)j / ((float)_size / (_size * _fieldmultiplier));
				_vertices.emplace_back(glm::vec3(x, y, z));
				_blendmapCoords.emplace_back(glm::vec2(x / 512, z / 512));
				_texCoords.emplace_back(glm::vec2(x, z));
				_normals.emplace_back(glm::vec3(0, 0, 0));
				_isPicked.emplace_back(glm::vec3(1.0, 1.0, 1.0));
				
				if ((j != _size) && (i != _size))
				{
					const int row1 = j * (_size + 1);
					const int row2 = (j + 1) * (_size + 1);

					//Triangle 1
					_indices.emplace_back(glm::uvec3(row1 + i, row1 + i + 1, row2 + i + 1));

					//Triangle 2
					_indices.emplace_back(glm::uvec3(row1 + i, row2 + i + 1, row2 + i));
				}

				//2 Dim-Array for cursor
				_index++;				
				int ix = (int)x;
				int iz = (int)z;
				if(ix >=0 && ix < 512 && iz >= 0 && iz < 512)
					_twoDimArray[ix][iz] = _index;
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

	void calculate_normals_per_triangle()
	{
		std::vector<Triangle> triangles;
		std::vector<glm::vec3> unmapped_normals;
		float computations = 0.0f;
		const float first_computations = _indices.size();
		const float second_computations = _size * _size * 2;
		const float third_computations = _indices.size();
		int progress = 0;
		std::cout << "\n";

		for (glm::uvec3 uvec3 : _indices)
		{
			auto index0 = uvec3.x;
			auto index1 = uvec3.y;
			auto index2 = uvec3.z;

			Point P0(_vertices.at(index0).x, _vertices.at(index0).y, _vertices.at(index0).z);
			Point P1(_vertices.at(index1).x, _vertices.at(index1).y, _vertices.at(index1).z);
			Point P2(_vertices.at(index2).x, _vertices.at(index2).y, _vertices.at(index2).z);

			triangles.emplace_back(P0, P1, P2);

			computations++;
			progress = (int)((computations / first_computations) * 100.0f);
			std::cout << "Triangles are getting calculated (%): " << progress << "\r";
		}
		computations = 0;
		std::cout << "Triangles are getting calculated (%): 100\n";

		for (Triangle t : triangles)
		{
			float u0 = t._P1._x - t._P0._x;
			float u1 = t._P1._y - t._P0._y;
			float u2 = t._P1._z - t._P0._z;
			glm::vec3 U(u0, u1, u2);

			float v0 = t._P2._x - t._P0._x;
			float v1 = t._P2._y - t._P0._y;
			float v2 = t._P2._z - t._P0._z;
			glm::vec3 V(v0, v1, v2);

			unmapped_normals.emplace_back(glm::cross(U, V));

			computations++;
			progress = (int)((computations / second_computations) * 100.0f);
			std::cout << "Normals are getting calculated (%): " << progress << "\r";
		}
		computations = 0;
		std::cout << "Normals are getting calculated (%): 100\n";

		for (int i = 0; i < _indices.size(); i++)
		{
			auto index0 = _indices.at(i).x;
			auto index1 = _indices.at(i).y;
			auto index2 = _indices.at(i).z;
			_normals.at(index0) = unmapped_normals.at(i);
			_normals.at(index1) = unmapped_normals.at(i);
			_normals.at(index2) = unmapped_normals.at(i);

			computations++;
			progress = (int)((computations / third_computations) * 100.0f);
			std::cout << "Normals are getting mapped (%): " << progress << "\r";
		}
		computations = 0;
		std::cout << "Normals are getting mapped (%): 100\n";

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
		_blendmapCoordsSize = _blendmapCoords.size() * sizeof(glm::vec2);
		_normalSize = _normals.size() * sizeof(glm::vec3);
		_verticesToRender = (GLsizei)_indices.size() * 3;
		_isPickedSize = _isPicked.size() * sizeof(glm::vec3);
	}

	float getHeightValueBuffered(const int& x, const int& z) const
	{
		return _heightmap->getPixelValueBuffered(x, z);
	}

	float getHeightValueUnbuffered(const int& x, const int& z) const
	{
		return _heightmap->getPixelValueUnbuffered(x, z);
	}
};