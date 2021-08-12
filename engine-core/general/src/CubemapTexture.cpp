#include "CubemapTexture.hpp"

namespace Core
{
    // ----- Public -----

    CubemapTexture::CubemapTexture(std::vector<const char*>& faces)
        : _textureID(0)
    {
        GLCall(glGenTextures(1, &_textureID));
        Bind();

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char* localBuffer = stbi_load(faces[i], &width, &height, &nrChannels, 0);
            if(localBuffer)
            {
                GLenum format = 0;
                if(nrChannels == 1)
                    format = GL_RED;
                else if(nrChannels == 3)
                    format = GL_RGB;
                else if(nrChannels == 4)
                    format = GL_RGBA;
                else
                    LOG(ERROR) << "Imageformat is not supported: " << faces[i];

                if(format != 0)
                {
                    GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, localBuffer));
                    LOG(INFO) << "Successfully load cubemap texture at: " << faces[i];
                    stbi_image_free(localBuffer);
                }
            }
            else
            {
                LOG(ERROR) << "Failed to load cubemap texture at: " << faces[i];
                stbi_image_free(localBuffer);
            }
        }

        //Texture parameters
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    }

    CubemapTexture::~CubemapTexture()
    {
        GLCall(glDeleteTextures(1, &_textureID));
    }

    void CubemapTexture::Bind() const
    {
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID));
    }

    void CubemapTexture::Unbind() const
    {
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }
}