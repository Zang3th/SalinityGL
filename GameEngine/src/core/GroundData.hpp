#pragma once

#include "RawData.hpp"
#include "Heightmap.hpp"

class GroundData : public RawData
{
private:
	unsigned int _size;
	Heightmap* _heightmap = nullptr;
	Heightmap* _terrainmap = nullptr;	

public:
	GroundData(int size)
		: _size(size)
	{
		init();
	}

	void init() override
	{
		_heightmap = new Heightmap("res/heightmaps/simple_noise_128.jpg", 0);
		_terrainmap = new Heightmap("res/heightmaps/terrain_map.jpg", 1);		

		for (int j = 0; j <= _size; ++j)
		{
			for (int i = 0; i <= _size; ++i)
			{
				float x = (float)i / ((float)_size / (_size * 6));
				float y = _heightmap->getPixelValue(i, j, 1.0f);
				float z = (float)j / ((float)_size / (_size * 6));
				_vertices.emplace_back(glm::vec3(x, y, z));
				_texCoords.emplace_back(glm::vec2(x, z));

				glm::vec3 terrain_color = _terrainmap->getColorValue(i, j);
				_terrainColors.push_back(terrain_color);

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
		setParameters();
	}

	void setParameters() override
	{
		_verticeSize = _vertices.size() * sizeof(glm::vec3);
		_indiceSize = _indices.size() * sizeof(glm::uvec3);
		_texCoordSize = _texCoords.size() * sizeof(glm::vec2);
		_verticesToRender = (GLsizei)_indices.size() * 3;
		_terrainColorSize = _terrainColors.size() * sizeof(glm::vec3);
	}
};