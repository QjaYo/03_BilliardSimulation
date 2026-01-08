#version 330 core

layout (location = 0) in vec3 vPosition;

out vec3 fTexCoords;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;

void main()
{
  fTexCoords = vPosition;

  mat4 staticViewMatrix = mat4(mat3(viewMatrix));
  //vec4 pos = projMatrix * viewMatrix * vec4(vPosition, 1.0);
  vec4 pos = projMatrix * staticViewMatrix * vec4(vPosition, 1.0);
  gl_Position = pos.xyww;
}