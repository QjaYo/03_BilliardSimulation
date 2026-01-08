#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "mesh.h"
#include "material.h"
#include "shader.h"

struct TransformData
{
  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 rotation = glm::vec3(0.0f);
  glm::vec3 scale = glm::vec3(1.0f);
};

struct RenderData
{
  const Shader *shader = nullptr;
  const Mesh *mesh = nullptr;
  const Material *material = nullptr;
};

struct PhysicsData
{
  float mass = 1.0f;
  glm::vec3 vel = glm::vec3(0.0f);
  glm::vec3 acc = glm::vec3(0.0f);
  glm::vec3 force = glm::vec3(0.0f);
  float restitution = 0.0f;
};

class Component {
public:
  std::vector<Component *> children;
  Component *parent = nullptr;

  TransformData transform;
  RenderData render;
  PhysicsData physics;

  Component() = default;
  Component(const TransformData &transform, const RenderData &render, const PhysicsData &physics);
  ~Component();
  void draw(const Shader &shader, const glm::mat4 &parentMatrix);
  void addChild(Component *child);
};