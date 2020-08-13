#pragma once

#include "stb_image/stb_image.h"
#include "OpenGLErrorManager.hpp"

class Texture
{ 
private:
	unsigned int _RendererID;
	const char* _Filepath;
	unsigned char* _LocalBuffer;
	int _Width, _Height, _BPP;
	
public:
	Texture()
	{
		
	}
	
	Texture(const char* path, unsigned int texSlot = 0)
		: _RendererID(0), _Filepath(path), _LocalBuffer(nullptr), _Width(0), _Height(0), _BPP(0)
	{		
		stbi_set_flip_vertically_on_load(1);
		_LocalBuffer = stbi_load(_Filepath, &_Width, &_Height, &_BPP, 0);

		if (_LocalBuffer)
		{
			GLenum format;
			if (_BPP == 1)
				format = GL_RED;
			else if (_BPP == 3)
				format = GL_RGB;
			else if (_BPP == 4)
				format = GL_RGBA;
			
			GLCall(glGenTextures(1, &_RendererID));
			GLCall(glActiveTexture(GL_TEXTURE0 + texSlot));
			GLCall(glBindTexture(GL_TEXTURE_2D, _RendererID));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, _Width, _Height, 0, format, GL_UNSIGNED_BYTE, _LocalBuffer));
			
			GLCall(glGenerateMipmap(GL_TEXTURE_2D));			
			
			/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/
			
			spdlog::info("Texture loaded successfully: {}", _Filepath);
			stbi_image_free(_LocalBuffer);
		}
		else
		{
			spdlog::error("Texture failed to load: {}", _Filepath);
			stbi_image_free(_LocalBuffer);
		}
	}

	~Texture()
	{
		GLCall(glDeleteTextures(1, &_RendererID));
	}

	void bind(unsigned int slot = 0) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, _RendererID));
	}

	void unbind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
};