#include "shapeGenerator.h"

#include <glm/glm.hpp>

#include "constant.h"

Mesh *ShapeGenerator::createQuad()
{
  std::vector<Vertex> vertices =
  {
    Vertex(glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f), glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec4(0.5f, -0.5f, 0.0f, 1.0f), glm::vec4(0.5f, -0.5f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec4(0.5f,  0.5f, 0.0f, 1.0f), glm::vec4(0.5f,  0.5f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f), glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f))
  };

  std::vector<unsigned int> indices =
  {
    0, 1, 2,
    2, 3, 0
  };

  return new Mesh(vertices, indices);
}

Mesh *ShapeGenerator::createCube()
{
  std::vector<Vertex> vertices =
  {
    // front face
    Vertex(glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f), glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec4(0.5f, -0.5f,  0.5f, 1.0f), glm::vec4(0.5f, -0.5f,  0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec4(0.5f,  0.5f,  0.5f, 1.0f), glm::vec4(0.5f,  0.5f,  0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f), glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
    // back face
    Vertex(glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec4(0.5f, -0.5f, -0.5f, 1.0f), glm::vec4(0.5f, -0.5f, -0.5f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec4(0.5f,  0.5f, -0.5f, 1.0f), glm::vec4(0.5f,  0.5f, -0.5f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)),
    Vertex(glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f), glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
    // left face
    Vertex(glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f), glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f), glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f), glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
    // right face
    Vertex(glm::vec4(0.5f, -0.5f, -0.5f, 1.0f), glm::vec4(0.5f, -0.5f, -0.5f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec4(0.5f, -0.5f,  0.5f, 1.0f), glm::vec4(0.5f, -0.5f,  0.5f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec4(0.5f,  0.5f,  0.5f, 1.0f), glm::vec4(0.5f,  0.5f,  0.5f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
    Vertex(glm::vec4(0.5f,  0.5f, -0.5f, 1.0f), glm::vec4(0.5f,  0.5f, -0.5f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
    // top face
    Vertex(glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f), glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
    Vertex(glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f), glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec4(0.5f,  0.5f,  0.5f, 1.0f), glm::vec4(0.5f,  0.5f,  0.5f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec4(0.5f,  0.5f, -0.5f, 1.0f), glm::vec4(0.5f,  0.5f, -0.5f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
    // bottom face
    Vertex(glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f), glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec4(0.5f, -0.5f,  0.5f, 1.0f), glm::vec4(0.5f, -0.5f,  0.5f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec4(0.5f, -0.5f, -0.5f, 1.0f), glm::vec4(0.5f, -0.5f, -0.5f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f))
  };

  std::vector<unsigned int> indices =
  {
    // front face
    0, 1, 2,
    2, 3, 0,
    // back face
    4, 5, 6,
    6, 7, 4,
    // left face
    8, 9,10,
    10,11, 8,
    // right face
    12,13,14,
    14,15,12,
    // top face
    16,17,18,
    18,19,16,
    // bottom face
    20,21,22,
    22,23,20
  };

  return new Mesh(vertices, indices);
}

Mesh *ShapeGenerator::createSphere()
{
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  unsigned int sectors = 32;
  unsigned int stacks = 32;

  glm::vec4 position = glm::vec4(1.0f);
  glm::vec4 color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
  glm::vec3 normal;
  glm::vec2 texCoords;

  float sectorStep = 2 * PI / sectors;
  float stackStep = PI / stacks;
  float sectorAngle, stackAngle;

  for (unsigned int i = 0; i <= stacks; i++)
  {
    stackAngle = PI / 2 - i * stackStep;
    float xy = cosf(stackAngle);
    position.z = sinf(stackAngle);

    for (unsigned int j = 0; j <= sectors; j++)
    {
      sectorAngle = j * sectorStep;

      position.x = xy * cos(sectorAngle);
      position.y = xy * sin(sectorAngle);

      normal.x = position.x;
      normal.y = position.y;
      normal.z = position.z;

      texCoords.s = (float)j / sectors;
      texCoords.t = (float)i / stacks;

      vertices.push_back(Vertex(position, position, normal, texCoords));
    }
  }

  unsigned int k1, k2;
  for (unsigned int i = 0; i < stacks; i++)
  {
    k1 = i * (sectors + 1);
    k2 = (i + 1) * (sectors + 1);

    for (unsigned int j = 0; j < sectors; j++, k1++, k2++)
    {
      if (i != 0)
      {
        indices.push_back(k1);
        indices.push_back(k2);
        indices.push_back(k1 + 1);
      }
      if (i != (stacks - 1))
      {
        indices.push_back(k1 + 1);
        indices.push_back(k2);
        indices.push_back(k2 + 1);
      }
    }
  }
  return new Mesh(vertices, indices);
}