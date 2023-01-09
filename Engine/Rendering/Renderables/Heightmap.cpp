#include "Heightmap.hpp"

namespace Engine
{
    // ----- Public -----

    Heightmap::Heightmap(const std::string& filepath)
    {
        //Load image
        int32 width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* localBuffer = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 3);
        stbi_set_flip_vertically_on_load(false);

        //Reserve space
        _heightmap.reserve(APP_SETTINGS.planeSize * APP_SETTINGS.planeSize * sizeof(float));

        if(localBuffer)
        {
            if(width < (int)APP_SETTINGS.planeSize || height < (int)APP_SETTINGS.planeSize)
            {
                std::string texInfo = "Texture too small: " + filepath;
                Logger::Error("Failed", "Heightmap", texInfo);
            }
            else if(nrChannels != 3)
            {
                std::string texInfo = "Texture doesn't have 3 channels: " + filepath;
                Logger::Error("Failed", "Heightmap", texInfo);
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

                        _heightmap.push_back(x + y + z);
                    }
                }

                std::string heightmapInfo = "(X: " + std::to_string(width) + ", Y: " + std::to_string(height) + ", Channels: " + std::to_string(nrChannels) + ")";
                Logger::Info("Loaded", "Heightmap", filepath);
                Logger::Info("", "", heightmapInfo);
            }
        }
        else
            Logger::Error("Failed", "Heightmap-Load.", filepath);

        stbi_image_free(localBuffer);
    }

    float Heightmap::GetValueAt(const uint32 x, const uint32 z) const
    {
        //Return value at given position with some special treatment for the edges

        if(x < APP_SETTINGS.planeSize && z < APP_SETTINGS.planeSize)
            return _heightmap.at(APP_SETTINGS.planeSize * x + z);

        else if(x-1 < APP_SETTINGS.planeSize && z < APP_SETTINGS.planeSize)
            return _heightmap.at(APP_SETTINGS.planeSize * (x-1) + z);

        else if(x < APP_SETTINGS.planeSize && z-1 < APP_SETTINGS.planeSize)
            return _heightmap.at(APP_SETTINGS.planeSize * x + (z-1));

        else if(x-1 < APP_SETTINGS.planeSize && z-1 < APP_SETTINGS.planeSize)
            return _heightmap.at(APP_SETTINGS.planeSize * (x-1) + (z-1));

        return 0;
    }
}