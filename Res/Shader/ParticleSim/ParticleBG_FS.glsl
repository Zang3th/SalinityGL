#version 450 core

in vec2 screenPos;

out vec4 fragColor;

uniform vec3 color;

void main()
{
    vec3 bgColor = color;

    //Create grid
    bgColor += clamp(step(mod(screenPos.x, 31.0), 1.0) + step(mod(screenPos.y, 31.0), 1.0), 0.0, 0.1);

    fragColor = vec4(bgColor, 1.0);
}