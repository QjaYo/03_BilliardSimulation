#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh.h"
#include "material.h"

class Object {
public:
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
  Material material;
  const Mesh *mesh;

  Object();
  Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, const Material &material, const Mesh *mesh);
  ~Object();
  void draw(const Shader &shader);
};