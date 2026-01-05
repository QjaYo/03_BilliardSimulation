#pragma once

#include <glad/glad.h>

#include <vector>

#include "shader.h"
#include "vertex.h"

class Mesh
{
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  unsigned int VAO;

  Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);
  ~Mesh();

  void draw(const Shader &shader) const;

private:
  unsigned int VBO, EBO;
  void setupMesh();
};