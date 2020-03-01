#pragma once

#include "stb_image/stb_image.h"
#include "OpenGLErrorManager.hpp"

class Cubemap
{
private:
    unsigned int _RendererID;

public:
	Cubemap(std::vector<std::string>& faces)
	{
        GLCall(glGenTextures(1, &_RendererID));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, _RendererID));
        stbi_set_flip_vertically_on_load(false);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                ));
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	}

    ~Cubemap()
    {
        GLCall(glDeleteTextures(1, &_RendererID));
    }

    void bind(unsigned int slot) const
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, _RendererID));
    }

    void unbind() const
    {
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }
};