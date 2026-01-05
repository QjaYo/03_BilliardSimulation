#pragma once

#include "mesh.h"

class ShapeGenerator {
public:
  static Mesh *createQuad();
  static Mesh *createCube();
  static Mesh *createSphere();
};