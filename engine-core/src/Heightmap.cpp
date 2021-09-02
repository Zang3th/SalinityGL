#include "Heightmap.hpp"

namespace Core
{
    // ----- Public -----

    Heightmap::Heightmap(const std::string& filepath)
    {
        int32 width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* localBuffer = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
        stbi_set_flip_vertically_on_load(false);

        if(localBuffer)
        {
            if(width < PLANE_SIZE || height < PLANE_SIZE)
            {
                LOG(ERROR) << "Failed:   Heightmap is too small | " << filepath;
            }
            else if(nrChannels != 3)
            {
                LOG(ERROR) << "Failed:   Heightmap doesn't have 3 channels | " << filepath;
            }
            else
            {
                //Save image into buffer
                uint32 count = 0;
                for(int32 i = 0; i < width; i++)
                {
                    for(int32 j = 0; j < height; j++)
                    {
                        float x = (float)localBuffer[count] / 255.0f;
                        float y = (float)localBuffer[count + 1] / 255.0f;
                        float z = (float)localBuffer[count + 2] / 255.0f;
                        _heightArray[i][j] = x + y + z;
                        count += 3;
                    }
                }

                LOG(INFO) << "Loaded:   Heightmap-Texture | " << filepath << " (X: " << width << " | Y: " << height << " | Channels: " << nrChannels << ")";
            }
        }
        else
        {
            LOG(ERROR) << "Failed:   Heightmap-Texture-Loading | " << filepath;
        }

        stbi_image_free(localBuffer);
    }

    float Heightmap::GetValueAt(const uint16 x, const uint16 z) const
    {
        if(x < PLANE_SIZE && z < PLANE_SIZE)
            return _heightArray[x][z];
        else if(x-1 < PLANE_SIZE && z < PLANE_SIZE)
            return _heightArray[x-1][z];
        else if(x < PLANE_SIZE && z-1 < PLANE_SIZE)
            return _heightArray[x][z-1];
        else if(x-1 < PLANE_SIZE && z-1 < PLANE_SIZE)
            return _heightArray[x-1][z-1];

        return 0;
    }
}