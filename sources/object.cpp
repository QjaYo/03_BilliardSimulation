#include "object.h"

Object::Object(const TransformData &transform, const RenderData &render, const PhysicsData &physics)
{
  this->transform = transform;
  this->render = render;
  this->physics = physics;
}

Object::~Object() {
  ;
}

void Object::draw(const Shader &shader)
{
  if (render.mesh == nullptr || render.material == nullptr)
  {
    return;
  }
  const Material& mat = *render.material;

  // Model Matrix
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::translate(modelMatrix, transform.position);
  modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // y axis
  modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // x axis
  modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // z axis
  modelMatrix = glm::scale(modelMatrix, transform.scale);
  shader.setMat4("modelMatrix", modelMatrix);

  // Material
  shader.setMaterial("material", mat);

  int unit = 0;

  shader.setBool("ACTIVATE_TEXTURE_DIFFUSE", mat.useDiffuseMaps);
  shader.setInt("n_TEXTURE_DIFFUSE", (int)mat.diffuseMaps.size());
  for (size_t i = 0; i < mat.diffuseMaps.size(); i++)
  {
    mat.diffuseMaps[i]->bind(unit);
    shader.setInt("TEXTURE_DIFFUSE[" + std::to_string(i) + "]", unit++);
  }

  shader.setBool("ACTIVATE_TEXTURE_SPECULAR", mat.useSpecularMaps);
  shader.setInt("n_TEXTURE_SPECULAR", (int)mat.specularMaps.size());
  for (size_t i = 0; i < mat.specularMaps.size(); i++)
  {
    mat.specularMaps[i]->bind(unit);
    shader.setInt("TEXTURE_SPECULAR[" + std::to_string(i) + "]", unit++);
  }

  shader.setBool("ACTIVATE_TEXTURE_NORMAL", mat.useNormalMaps);
  shader.setInt("n_TEXTURE_NORMAL", (int)mat.normalMaps.size());
  for (size_t i = 0; i < mat.normalMaps.size(); i++)
  {
    mat.normalMaps[i]->bind(unit);
    shader.setInt("TEXTURE_NORMAL[" + std::to_string(i) + "]", unit++);
  }

  shader.setBool("ACTIVATE_TEXTURE_HEIGHT", mat.useHeightMaps);
  shader.setInt("n_TEXTURE_HEIGHT", (int)mat.heightMaps.size());
  for (size_t i = 0; i < mat.heightMaps.size(); i++)
  {
    mat.heightMaps[i]->bind(unit);
    shader.setInt("TEXTURE_HEIGHT[" + std::to_string(i) + "]", unit++);
  }

  shader.setInt("n_TEXTURE_OTHER", (int)mat.otherMaps.size());
  for (size_t i = 0; i < mat.otherMaps.size(); i++)
  {
    mat.otherMaps[i]->bind(unit);
    shader.setInt("TEXTURE_OTHER[" + std::to_string(i) + "]", unit++);
  }

  render.mesh->draw(shader);
}