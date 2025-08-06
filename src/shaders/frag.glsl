#version 460 core
out vec4 FragColor;

in vec3 outcolor;

void main()
{
    FragColor = vec4(outcolor, 1.0f);
} 
