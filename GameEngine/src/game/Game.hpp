#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Shader.hpp"

#pragma once

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game
{
public:
    GameState _state;
    bool _keys[1024];
    unsigned int _width, _height;
    VertexArray* _vao = nullptr;
    VertexBuffer* _vbo = nullptr;
    Shader* _shader = nullptr;

    Game(unsigned int width, unsigned int height)
        : _state(GAME_ACTIVE), _keys(), _width(width), _height(height)
    {

    }

    ~Game()
    {

    }

    void init()
    {
        //Erstellt und bindet VAO
        _vao = new VertexArray();
        _vao->bind();

        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };

        _vbo = new VertexBuffer(vertices, sizeof(vertices));
        _vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        _vbo->unbind();
        _vao->unbind();

        _shader = new Shader("res/shader/breakout_vs.glsl", "res/shader/breakout_fs.glsl");
    }

    void update(float dt)
    {

    }

    void processInput(float dt)
    {

    }

    void render()
    {
        _shader->bind();
        _vao->bind();

        GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
    }
};