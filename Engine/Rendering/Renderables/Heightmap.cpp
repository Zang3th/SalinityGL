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
        _heightmap.reserve(RenderParams::planeSize * RenderParams::planeSize * sizeof(float));

        if(localBuffer)
        {
            if(width < (int)RenderParams::planeSize || height < (int)RenderParams::planeSize)
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
        { 
            Logger::Error("Failed", "Heightmap-Load.", filepath); 
        }

        stbi_image_free(localBuffer);
    }

    float Heightmap::GetValueAt(const uint32 x, const uint32 z) const
    {
        float value = 0.0f;

        //Return value at given position with some special treatment for the edges
        if(x < RenderParams::planeSize && z < RenderParams::planeSize)
        { 
            value = _heightmap.at(RenderParams::planeSize * x + z); 
        }
        else if(x-1 < RenderParams::planeSize && z < RenderParams::planeSize)
        { 
            value = _heightmap.at(RenderParams::planeSize * (x-1) + z); 
        }
        else if(x < RenderParams::planeSize && z-1 < RenderParams::planeSize)
        { 
            value = _heightmap.at(RenderParams::planeSize * x + (z-1)); 
        }
        else if(x-1 < RenderParams::planeSize && z-1 < RenderParams::planeSize)
        { 
            value = _heightmap.at(RenderParams::planeSize * (x-1) + (z-1)); 
        }

        return value;
    }
}
