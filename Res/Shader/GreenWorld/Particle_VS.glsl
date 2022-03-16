#version 450 core

layout(location = 0) in vec4 vertexIn;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    gl_Position = projection * model * vec4(vertexIn.xyz, 1.0f);
}