#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D tex;
uniform vec3 spriteColor;

void main()
{
    FragColor = vec4(spriteColor, 1.0f) * texture(tex, TexCoords);
}