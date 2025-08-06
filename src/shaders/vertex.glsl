#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;

out vec3 outcolor;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   // gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
   outcolor = color;
}
