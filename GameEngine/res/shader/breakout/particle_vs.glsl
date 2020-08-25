#version 330 core

layout(location = 0) in vec4 aPos; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    TexCoords = aPos.zw;
    gl_Position = projection * model * vec4(aPos.xy, 0.0, 1.0);
}