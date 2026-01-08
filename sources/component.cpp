#include "component.h"

Component::Component(const TransformData &transform, const RenderData &render, const PhysicsData &physics)
{
  this->transform = transform;
  this->render = render;
  this->physics = physics;
}

Component::~Component() {
  ;
}

void Component::draw(const Shader &shader, const glm::mat4 &parentMatrix)
{
  // Model Matrix
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::translate(modelMatrix, transform.position);
  modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // y axis
  modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // x axis
  modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // z axis
  modelMatrix = glm::scale(modelMatrix, transform.scale);
  modelMatrix = parentMatrix * modelMatrix;
  shader.setMat4("modelMatrix", modelMatrix);

  bool drawable = render.mesh != nullptr && render.material != nullptr;
  if (drawable)
  {
    const Material &material = *render.material;
    shader.setMaterial("material", material);

    int unit = 0;

    shader.setBool("ACTIVATE_TEXTURE_DIFFUSE", material.useDiffuseMaps);
    shader.setInt("n_TEXTURE_DIFFUSE", (int)material.diffuseMaps.size());
    for (size_t i = 0; i < material.diffuseMaps.size(); i++)
    {
      material.diffuseMaps[i]->bind(unit);
      shader.setInt("TEXTURE_DIFFUSE[" + std::to_string(i) + "]", unit++);
    }

    shader.setBool("ACTIVATE_TEXTURE_SPECULAR", material.useSpecularMaps);
    shader.setInt("n_TEXTURE_SPECULAR", (int)material.specularMaps.size());
    for (size_t i = 0; i < material.specularMaps.size(); i++)
    {
      material.specularMaps[i]->bind(unit);
      shader.setInt("TEXTURE_SPECULAR[" + std::to_string(i) + "]", unit++);
    }

    shader.setBool("ACTIVATE_TEXTURE_NORMAL", material.useNormalMaps);
    shader.setInt("n_TEXTURE_NORMAL", (int)material.normalMaps.size());
    for (size_t i = 0; i < material.normalMaps.size(); i++)
    {
      material.normalMaps[i]->bind(unit);
      shader.setInt("TEXTURE_NORMAL[" + std::to_string(i) + "]", unit++);
    }

    shader.setBool("ACTIVATE_TEXTURE_HEIGHT", material.useHeightMaps);
    shader.setInt("n_TEXTURE_HEIGHT", (int)material.heightMaps.size());
    for (size_t i = 0; i < material.heightMaps.size(); i++)
    {
      material.heightMaps[i]->bind(unit);
      shader.setInt("TEXTURE_HEIGHT[" + std::to_string(i) + "]", unit++);
    }

    shader.setInt("n_TEXTURE_OTHER", (int)material.otherMaps.size());
    for (size_t i = 0; i < material.otherMaps.size(); i++)
    {
      material.otherMaps[i]->bind(unit);
      shader.setInt("TEXTURE_OTHER[" + std::to_string(i) + "]", unit++);
    }

    render.mesh->draw(shader);
  }

  for (auto &child : children)
  {
    child->draw(shader, modelMatrix);
  }
}

void Component::addChild(Component *child)
{
  if (child != nullptr)
    child->parent = this;
    children.push_back(child);
}