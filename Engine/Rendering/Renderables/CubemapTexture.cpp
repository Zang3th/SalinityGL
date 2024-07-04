#include "CubemapTexture.hpp"

namespace Engine
{
    // ----- Public -----

    CubemapTexture::CubemapTexture(const std::array<const char*, 6>& faces)
        :   _textureID(0)
    {
        stbi_set_flip_vertically_on_load(false);
        GLCall(glGenTextures(1, &_textureID))
        Bind();

        int32 width, height, nrChannels;
        for (uint32 i = 0; i < faces.size(); i++)
        {
            unsigned char* localBuffer = stbi_load(faces[i], &width, &height, &nrChannels, 0);
            if(localBuffer)
            {
                GLenum format = 0;
                if(nrChannels == 1)
                { 
                    format = GL_RED; 
                }
                else if(nrChannels == 3)
                { 
                    format = GL_RGB; 
                }
                else if(nrChannels == 4)
                { 
                    format = GL_RGBA; 
                }
                else
                { 
                    Logger::Error("Failed", "Cubemap-Format", faces[i]); 
                }

                if(format != 0)
                {
                    GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, localBuffer))

                    std::string cubemapInfo = "(X: " + std::to_string(width) + ", Y: " + std::to_string(height) + ", Channels: " + std::to_string(nrChannels) + ")";
                    Logger::Info("Loaded", "Cubemap", faces[i]);
                    Logger::Info("", "", cubemapInfo);
                }
            }
            else
            { 
                Logger::Error("Failed", "Cubemap-Load.", faces[i]); 
            }

            stbi_image_free(localBuffer);
        }

        //Texture parameters
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR))
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE))
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE))
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE))

        stbi_set_flip_vertically_on_load(true);
    }

    CubemapTexture::~CubemapTexture()
    {
        GLCall(glDeleteTextures(1, &_textureID))
    }

    void CubemapTexture::Bind() const
    {
        GLCall(glActiveTexture(GL_TEXTURE0))
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID))
    }

    void CubemapTexture::Unbind() const
    {
        GLCall(glBindTexture(GL_TEXTURE_2D, 0))
    }
}
