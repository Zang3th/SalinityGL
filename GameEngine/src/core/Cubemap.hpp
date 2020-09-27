#pragma once

#include "stb_image/stb_image.h"
#include "OpenGLErrorManager.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "Camera.hpp"

class CubemapTexture
{
private:
    unsigned int _RendererID;
    friend class Cubemap;

    CubemapTexture(std::vector<const char*>& faces)
    {
        GLCall(glGenTextures(1, &_RendererID));
        GLCall(glActiveTexture(GL_TEXTURE0));
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

    ~CubemapTexture()
    {
        GLCall(glDeleteTextures(1, &_RendererID));
    }

    void bind() const
    {
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, _RendererID));
    }

    void unbind() const
    {
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }
};

class Cubemap 
{
private:
    CubemapTexture* _texture = nullptr;
    Shader* _shader = nullptr;
    VertexBuffer* _vbo = nullptr;
    VertexArray* _vao = nullptr;
    glm::mat4 _projection, _view;
    Camera* _camera = nullptr;

    void initData() 
    {
        float skyboxVertices[] = 
        {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };
    
        //Create and bind vao
        _vao = new VertexArray();
        _vao->bind();

        //Create vbo and configure vao
        _vbo = new VertexBuffer(&skyboxVertices, sizeof(skyboxVertices));
        _vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        _vbo->unbind();
        _vao->unbind();
    }

public:
    Cubemap(std::vector<const char*>& faces, Shader* shader, Camera* camera, float width, float height, float renderDistance)
        : _shader(shader), _camera(camera)
    {
        _texture = new CubemapTexture(faces);
        _projection = glm::perspective(glm::radians(_camera->Zoom), width / height, 0.1f, renderDistance);
        initData();
    }

    ~Cubemap() 
    {
        delete _texture;
        delete _vbo;
        delete _vao;
    }

    void render() 
    {
        //Deactivate depth mask
        GLCall(glDepthFunc(GL_LEQUAL););

        //Matrices        
        _view = glm::mat4(glm::mat3(_camera->GetViewMatrix()));

        _shader->bind();

        //Set uniforms
        _shader->SetUniformMat4f("projection", _projection);
        _shader->SetUniformMat4f("view", _view);
        _shader->SetUniform1i("textureSampler", 0);

        //Set texture
        _texture->bind();

        //Bind vao
        _vao->bind();

        //Render
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

        //Reactivate depth mask
        GLCall(glDepthFunc(GL_LESS));
    }
};