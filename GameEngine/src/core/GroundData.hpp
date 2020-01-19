#pragma once

#include "RawData.hpp"
#include "Heightmap.hpp"

class GroundData : public RawData
{
private:
	unsigned int _size;
	Heightmap* _heightmap = nullptr;

public:
	GroundData(int size)
		: _size(size)
	{
		init();
	}

	void init() override
	{
		_heightmap = new Heightmap("res/heightmaps/simple_noise_512.bmp", 0);
		//_heightmap->display();

		for (int j = 0; j <= _size; ++j)
		{
			for (int i = 0; i <= _size; ++i)
			{
				float x = (float)i / ((float)_size / (_size * 5));
				float y = _heightmap->getPixelValue(i, j, 3.0f);
				//std::cout << y << std::endl;
				float z = (float)j / ((float)_size / (_size * 5));
				_vertices.emplace_back(glm::vec3(x, y, z));
				_texCoords.emplace_back(glm::vec2(x, z));

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
	}
};
