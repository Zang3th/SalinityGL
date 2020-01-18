#pragma once

#include "RawData.hpp"

class GroundData : public RawData
{
private:
	unsigned int _size;
public:
	GroundData(int size)
		: _size(size)
	{
		init();
	}

	void init() override
	{
		for (int j = 0; j <= _size; ++j)
		{
			for (int i = 0; i <= _size; ++i)
			{
				float x = (float)i / ((float)_size / (_size * 20));
				float y = 0;
				float z = (float)j / ((float)_size / (_size * 20));
				_vertices.push_back(x);
				_vertices.push_back(y);
				_vertices.push_back(z);
				_vertices.push_back(x);
				_vertices.push_back(z);

				if ((j != _size) && (i != _size))
				{
					const int row1 = j * (_size + 1);
					const int row2 = (j + 1) * (_size + 1);

					// triangle 1
					_indices.push_back(row1 + i);
					_indices.push_back(row1 + i + 1);
					_indices.push_back(row2 + i + 1);

					// triangle 2
					_indices.push_back(row1 + i);
					_indices.push_back(row2 + i + 1);
					_indices.push_back(row2 + i);
				}
			}
		}
		setParameters();
	}

	void setParameters() override
	{
		_verticeSize = _vertices.size() * sizeof(float);
		_indiceSize = _indices.size() * sizeof(unsigned int);
		_verticesToRender = _indices.size() * sizeof(unsigned int);
	}
};
