#version 460 core
out vec4 FragColor;

uniform float opacity;

in vec3 outcolor;

void main()
{
    FragColor = vec4(outcolor, opacity);
} 
