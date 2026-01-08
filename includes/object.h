#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

#include "component.h"

class Object
{
public:
  std::string name;
  Component *root;

  Object() = default;
  Object(const std::string &name, Component *root);
  ~Object();

  void draw(const Shader &shader);
};