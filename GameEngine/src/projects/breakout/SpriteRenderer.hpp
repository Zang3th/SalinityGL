#pragma once

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "glm/gtc/matrix_transform.hpp"

class SpriteRenderer
{
private:
	VertexArray* _vao = nullptr;
	VertexBuffer* _vbo = nullptr;
	Shader* _shader = nullptr;
    unsigned int _width, _height;
	
public:
    void initRenderData()
    {
        //Erstellt und bindet VAO
        _vao = new VertexArray();
        _vao->bind();

        float vertices[] = 
        {
            // pos      // tex
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        };

        _vbo = new VertexBuffer(vertices, sizeof(vertices));
        _vao->DefineAttributes(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

        _vbo->unbind();
        _vao->unbind();
    }
	
    SpriteRenderer(Shader* shader, const unsigned int& width, const unsigned int& height)
	    : _shader(shader), _width(width), _height(height)
    {
        this->initRenderData();
    }

    ~SpriteRenderer()
    {
        delete _vao;
        delete _vbo;
    }
    
    void DrawSprite(Texture* texture, glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color)
    {
        _shader->bind();

    	//Projection-Matrix
        glm::mat4 projection = glm::ortho(0.0f, (float)_width, (float)_height, 0.0f, -1.0f, 1.0f);

    	//Model transformations
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

        model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

    	//Set Uniforms
        _shader->SetUniformMat4f("projection", projection);    	
        _shader->SetUniformMat4f("model", model);    	
        _shader->SetUniformVec3("spriteColor", color);

    	//Set texture
        texture->bind();

    	//Bind vao
        _vao->bind();

    	//Render quad
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
    }
};