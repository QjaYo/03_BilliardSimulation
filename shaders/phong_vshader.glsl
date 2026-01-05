#version 330 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec2 vTexCoords;

out vec4 fPosition;
out vec4 fColor;
out vec3 fNormal;
out float fHeight;
out vec2 fTexCoords;

// global uniforms
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main()
{
  gl_Position = projMatrix * viewMatrix * modelMatrix * vPosition;

  fPosition = modelMatrix * vPosition;
  fColor = vPosition * 0.5 + 0.5;
  fNormal = (modelMatrix * vec4(vNormal, 0.0f)).xyz; // non-uniform scaling -> should use normal matrix
  fHeight = 0.0f;
  fTexCoords = vTexCoords;
}