#include "Heightmap.hpp"

namespace Core
{
    // ----- Public -----

    Heightmap::Heightmap(const std::string& filepath)
    {
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* localBuffer = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
        stbi_set_flip_vertically_on_load(false);

        if(localBuffer)
        {
            if(width < PLANE_SIZE || height < PLANE_SIZE)
            {
                LOG(ERROR) << "Heightmap is too small: " << filepath;
            }
            else if(nrChannels != 3)
            {
                LOG(ERROR) << "Heightmap doesn't have 3 channels: " << filepath;
            }
            else
            {
                //Save image into buffer
                int count = 0;
                for(int i = 0; i < width; i++)
                {
                    for(int j = 0; j < height; j++)
                    {
                        float x = (float)localBuffer[count] / 255.0f;
                        float y = (float)localBuffer[count + 1] / 255.0f;
                        float z = (float)localBuffer[count + 2] / 255.0f;
                        _heightArray[i][j] = x + y + z;
                        count+=3;
                    }
                }
                LOG(INFO) << "Successfully load heightmap texture at: " << filepath << " (X: " << width << " | Y: " << height << " | Channel: " << nrChannels << ")";
            }
        }
        else
        {
            LOG(ERROR) << "Failed to load heightmap texture at: " << filepath;
        }

        stbi_image_free(localBuffer);
    }

    float Heightmap::GetValueAt(const unsigned int x, const unsigned int z) const
    {
        if(x < PLANE_SIZE && z < PLANE_SIZE)
            return _heightArray[x][z];

        return 0;
    }
}