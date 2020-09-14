#pragma once

#include "GameObject.hpp"
#include <vector>
#include <string>

class GameLevelCreator
{
private:
    SpriteRenderer* _spriteRenderer = nullptr;
    unsigned int _gameWidth, _gameHeight;
    Texture* _block_tex, * _solid_block_tex;
	
public:
    std::vector<GameObject> _bricks;
	
	GameLevelCreator(SpriteRenderer* spriteRenderer, const unsigned int& width, const unsigned int& height, Texture* block_tex, Texture* solid_block_tex)
		: _spriteRenderer(spriteRenderer), _gameWidth(width), _gameHeight(height), _block_tex(block_tex),  _solid_block_tex(solid_block_tex)
	{
		
	}

    ~GameLevelCreator()
	{
        
	}
	
    void generateLevel(const char* level_filepath)
    {		    
        //Load from file
        unsigned int tileCode;
        std::string line;
        std::ifstream fstream(level_filepath);
        std::vector<std::vector<unsigned int>> tileData;
		
        if (fstream)
        {
            while (std::getline(fstream, line)) //Read each line from level file
            {
                std::istringstream sstream(line);
                std::vector<unsigned int> row;
                while (sstream >> tileCode) //Read each word separated by spaces
                    row.push_back(tileCode);
                tileData.push_back(row);
            }
        }
		
        if (!tileData.empty())
        {
            float height = tileData.size();
            float width = tileData[0].size();
            float brick_width = _gameWidth / width;
            float brick_height = brick_width / 1.5f;

            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    if (tileData[y][x] == 1) //Solid
                    {
                        glm::vec2 pos(brick_width * x, brick_height * y);
                        glm::vec2 size(brick_width, brick_height);
                    	
                        _bricks.emplace_back(pos, size, glm::vec2(0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.7f), 0.0f, _solid_block_tex, _spriteRenderer, true);
                    }

                    else if (tileData[y][x] > 1)
                    {
                        glm::vec3 color = glm::vec3(1.0f);       //White

                        if (tileData[y][x] == 2)
                            color = glm::vec3(0.2f, 0.6f, 1.0f); //Blue
                        else if (tileData[y][x] == 3)
                            color = glm::vec3(0.0f, 0.7f, 0.0f); //Green
                        else if (tileData[y][x] == 4)
                            color = glm::vec3(0.9f, 0.9f, 0.2f); //Yellow
                        else if (tileData[y][x] == 5)
                            color = glm::vec3(1.0f, 0.5f, 0.0f); //Orange

                        glm::vec2 pos(brick_width * x, brick_height * y);
                        glm::vec2 size(brick_width, brick_height);

                        _bricks.emplace_back(pos, size, glm::vec2(0.0f, 0.0f), color, 0.0f, _block_tex, _spriteRenderer);
                    }
                }
            }
        }	        
    }

	void renderLevel()
	{
		for(auto brick: _bricks)
		{
			if (!brick._destroyed)
				brick.Draw();
		}
	}
};