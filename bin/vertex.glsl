#version 450 core

layout (location = 0) in vec4 vertexPosition;

void main(void)
{
    gl_Position = vertexPosition;
}

