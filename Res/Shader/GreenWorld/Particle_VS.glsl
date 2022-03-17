#version 450 core

layout(location = 0) in vec2 vertexIn;

uniform mat4 modelView;
uniform mat4 projection;

void main()
{
    gl_Position = projection * modelView * vec4(vertexIn, 0.0, 1.0f);
}