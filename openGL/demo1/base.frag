#version 410 core
smooth in vec4 vertOutColor;
layout (location = 0) out vec4 outColor;
void main()
{
    outColor = vertOutColor;
}
