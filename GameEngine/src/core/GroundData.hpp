#pragma once

#include "RawData.hpp"
#include "Heightmap.hpp"

class GroundData : public RawData
{
private:
	unsigned int _size, _fieldmultiplier;
	Heightmap* _heightmap = nullptr;

public:
	GroundData(int size, int fieldmultiplier)
		: _size(size), _fieldmultiplier(fieldmultiplier)
	{
		init();
	}

	void init() override
	{
		_heightmap = new Heightmap("res/maps/heightmap_256.bmp", 0);

		for (int j = 0; j <= _size; ++j)
		{
			for (int i = 0; i <= _size; ++i)
			{
				float x = (float)i / ((float)_size / (_size * _fieldmultiplier));
				float y = _heightmap->getPixelValue(i, j, 6.0f);
				float z = (float)j / ((float)_size / (_size * _fieldmultiplier));
				_vertices.emplace_back(glm::vec3(x, y, z));

				_blendmapCoords.emplace_back(glm::vec2(x / 512, z / 512));
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
		_blendmapCoordsSize = _blendmapCoords.size() * sizeof(glm::vec2);
		_verticesToRender = (GLsizei)_indices.size() * 3;
	}

	float getHeightValue(int x, int y, float scale)
	{
		return _heightmap->getPixelValue(x, y, scale);
	}
};