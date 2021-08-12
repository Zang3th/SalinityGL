#version 450 core

layout(location = 0) in vec3 vertexIn;

out vec3 texCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    texCoords = vertexIn;
    vec4 pos = projection * view * vec4(vertexIn, 1.0);
    gl_Position = pos.xyww; //To fail the depth test
}