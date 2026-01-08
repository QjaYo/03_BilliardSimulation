#pragma once

#include <glm/glm.hpp>

#include "component.h"
#include "object.h"

class Light
{
public:
  float intensity;
  Component component;

  Light() = default;
  Light(const Component &c, const float i);
};