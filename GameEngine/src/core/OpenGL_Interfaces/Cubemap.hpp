#pragma once

#include "stb_image/stb_image.h"
#include "OpenGLErrorManager.hpp"

class Cubemap
{
private:
    unsigned int _RendererID;

public:
	Cubemap(std::vector<const char*>& faces, const unsigned int& texSlot)
	{
        GLCall(glGenTextures(1, &_RendererID));
        GLCall(glActiveTexture(GL_TEXTURE0 + texSlot));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, _RendererID));
        stbi_set_flip_vertically_on_load(false);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load(faces[i], &width, &height, &nrChannels, 0);
            if (data)
            {
                GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                ));
                spdlog::info("Cubemap texture loaded successfully: {}", faces[i]);
                stbi_image_free(data);
            }
            else
            {
                spdlog::error("Cubemap texture failed to load at path: {}", faces[i]);
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