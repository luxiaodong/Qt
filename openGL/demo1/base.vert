#version 410 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
smooth out vec4 vertOutColor;
void main()
{
    gl_Position = vec4(inPosition, 1.0);
    vertOutColor = vec4(inColor, 1.0);
}
