#pragma once
#include <glm/glm.hpp>

struct Vertex
{
  glm::vec4 position;
  glm::vec4 color;
  glm::vec3 normal;
  glm::vec2 texCoords;

  Vertex(
    glm::vec4 p = glm::vec4(0.0f),
    glm::vec4 c = glm::vec4(1.0f),
    glm::vec3 n = glm::vec3(0.0f),
    glm::vec2 t = glm::vec2(0.0f)
  ): position(p), color(c), normal(n), texCoords(t) {}
};