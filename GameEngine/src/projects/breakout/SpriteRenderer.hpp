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
    glm::mat4 _projection;
	
    void initRenderData()
    {
        //Create and bind vao
        _vao = new VertexArray();
        _vao->bind();

        float vertices[] =
        {
            //Position  //Texture
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

	void matrixSetUP()
    {
        //Projection-Matrix
        _projection = glm::ortho(0.0f, (float)_width, (float)_height, 0.0f, -1.0f, 1.0f);
    }
	
public:  
    SpriteRenderer(Shader* shader, const unsigned int& width, const unsigned int& height)
	    : _shader(shader), _width(width), _height(height)
    {
        this->initRenderData();
        this->matrixSetUP();
    }

    ~SpriteRenderer()
    {
        delete _vao;
        delete _vbo;
    }
    
    void DrawSprite(Texture* texture, glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color)
    {
        _shader->bind();

        //Model-Matrix
        glm::mat4 model = glm::mat4(1.0f);
    	
    	//Model transformations    	
        model = glm::translate(model, glm::vec3(position, 0.0f));  //First translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); //Move origin of rotation to center of quad
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); //Then rotate
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); //Move origin back

        model = glm::scale(model, glm::vec3(size, 1.0f)); //Last scale

    	//Set uniforms
        _shader->SetUniformMat4f("projection", _projection);
        _shader->SetUniformMat4f("model", model);    	
        _shader->SetUniformVec3("spriteColor", color);

    	//Set texture
        texture->bind();

    	//Bind vao
        _vao->bind();

    	//Render quad
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
    }

    glm::mat4 getProjectionMatrix() const
    {
        return _projection;
    }
};