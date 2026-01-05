#version 330 core

layout (location = 0) in vec4 vPosition;

out vec4 fPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main() 
{
    fPosition = modelMatrix * vPosition;
    gl_Position = projMatrix * viewMatrix * modelMatrix * vPosition;
}