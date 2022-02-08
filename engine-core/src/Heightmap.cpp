#include "Heightmap.hpp"

namespace Core
{
    // ----- Public -----

    Heightmap::Heightmap(const std::string& filepath)
    {
        //Load image
        int32 width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* localBuffer = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 3);
        stbi_set_flip_vertically_on_load(false);

        //Allocate buffer
        _heightArray = (float*)malloc(width * height * sizeof(float));
        LOG(INFO) << width * height * sizeof(float);

        if(localBuffer)
        {
            if(width < (int)PLANE_SIZE || height < (int)PLANE_SIZE)
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
                for(int32 i = 0; i < width; i++)
                {
                    for(int32 j = 0; j < height; j++)
                    {
                        unsigned char* pixelOffset = localBuffer + (width * i + j) * nrChannels;

                        float x = (float)pixelOffset[0] / 255.0f;
                        float y = (float)pixelOffset[1] / 255.0f;
                        float z = (float)pixelOffset[2] / 255.0f;

                        _heightArray[width * i + j] = x + y + z;
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

    Heightmap::~Heightmap()
    {
        free(_heightArray);
    }

    float Heightmap::GetValueAt(const uint32 x, const uint32 z) const
    {
        //Return value at given position with some special treatment for the edges

        if(x < PLANE_SIZE && z < PLANE_SIZE)
            return _heightArray[PLANE_SIZE * x + z];

        else if(x-1 < PLANE_SIZE && z < PLANE_SIZE)
            return _heightArray[PLANE_SIZE * (x-1) + z];

        else if(x < PLANE_SIZE && z-1 < PLANE_SIZE)
            return _heightArray[PLANE_SIZE * x + (z-1)];

        else if(x-1 < PLANE_SIZE && z-1 < PLANE_SIZE)
            return _heightArray[PLANE_SIZE * (x-1) + (z-1)];

        return 0;
    }
}