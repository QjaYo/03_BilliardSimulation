#pragma once

#include <glm/glm.hpp>

#include "object.h"

struct Light
{
  Object object;
  float intensity;

  Light(const Object &object, const float i);
};