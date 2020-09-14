#version 330 core

layout(location = 0) in vec3 PosIn;
layout(location = 1) in vec2 TexIn;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = TexIn;
    gl_Position = projection * view * model * vec4(PosIn, 1.0);
}