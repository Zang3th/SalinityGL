#version 440 core

layout(location = 0) in vec3 PosIn;
layout(location = 1) in vec2 TexIn;
layout(location = 2) in vec3 ColorIn;
layout(location = 3) in mat4 ModelIn;

out vec2 TexOut;
out vec3 ColorOut;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexOut = TexIn;
    ColorOut = ColorIn;
    gl_Position = projection * view * ModelIn * vec4(PosIn, 1.0);
}