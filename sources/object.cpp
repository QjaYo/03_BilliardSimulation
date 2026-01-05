#include "object.h"

Object::Object()
  : position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), scale(glm::vec3(1.0f)), material(), mesh(nullptr)
{}

Object::Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, const Material &material, const Mesh *mesh)
  : position(position), rotation(rotation), scale(scale), material(material), mesh(mesh) {}

Object::~Object() {
  ;
}

void Object::draw(const Shader &shader)
{
  // Model Matrix
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::translate(modelMatrix, this->position);
  modelMatrix = glm::rotate(modelMatrix, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // y axis
  modelMatrix = glm::rotate(modelMatrix, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // x axis
  modelMatrix = glm::rotate(modelMatrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // z axis
  modelMatrix = glm::scale(modelMatrix, this->scale);
  shader.setMat4("modelMatrix", modelMatrix);

  // Material
  shader.setMaterial("material", material);

  int unit = 0;

  shader.setBool("ACTIVATE_TEXTURE_DIFFUSE", material.useDiffuseMaps);
  shader.setInt("n_TEXTURE_DIFFUSE", (int)material.diffuseMaps.size());
  for (int i = 0; i < material.diffuseMaps.size(); i++)
  {
    material.diffuseMaps[i]->bind(unit);
    shader.setInt("TEXTURE_DIFFUSE[" + std::to_string(i) + "]", unit++);
  }

  shader.setBool("ACTIVATE_TEXTURE_SPECULAR", material.useSpecularMaps);
  shader.setInt("n_TEXTURE_SPECULAR", (int)material.specularMaps.size());
  for (int i = 0; i < material.specularMaps.size(); i++)
  {
    material.specularMaps[i]->bind(unit);
    shader.setInt("TEXTURE_SPECULAR[" + std::to_string(i) + "]", unit++);
  }

  shader.setBool("ACTIVATE_TEXTURE_NORMAL", material.useNormalMaps);
  shader.setInt("n_TEXTURE_NORMAL", (int)material.normalMaps.size());
  for (int i = 0; i < material.normalMaps.size(); i++)
  {
    material.normalMaps[i]->bind(unit);
    shader.setInt("TEXTURE_NORMAL[" + std::to_string(i) + "]", unit++);
  }

  shader.setBool("ACTIVATE_TEXTURE_HEIGHT", material.useHeightMaps);
  shader.setInt("n_TEXTURE_HEIGHT", (int)material.heightMaps.size());
  for (int i = 0; i < material.heightMaps.size(); i++)
  {
    material.heightMaps[i]->bind(unit);
    shader.setInt("TEXTURE_HEIGHT[" + std::to_string(i) + "]", unit++);
  }

  shader.setInt("n_TEXTURE_OTHER", (int)material.otherMaps.size());
  for (int i = 0; i < material.otherMaps.size(); i++)
  {
    material.otherMaps[i]->bind(unit);
    shader.setInt("TEXTURE_OTHER[" + std::to_string(i) + "]", unit++);
  }

  if (mesh == nullptr)
  {
    return;
  }

  mesh->draw(shader);
}