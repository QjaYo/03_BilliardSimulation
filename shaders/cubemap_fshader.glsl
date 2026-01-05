#version 330 core

in vec3 fTexCoords;

out vec4 FragColor;

uniform samplerCube cubemap;

void main()
{
  FragColor = texture(cubemap, fTexCoords);
}