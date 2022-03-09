#version 450 core
  
layout(location = 0) in vec4 vertexIn;

out vec2 texCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    texCoords   = vec2(vertexIn.zw);
    gl_Position = projection * model * vec4(vertexIn.xy, 0.0f, 1.0f);
}