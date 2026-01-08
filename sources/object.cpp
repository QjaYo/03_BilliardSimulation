#include "object.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Object::Object(const std::string &name, Component *root)
  : name(name), root(root) {}

Object::~Object()
{}

void Object::draw(const Shader &shader)
{
  if (root != nullptr)
  {
    root->draw(shader, glm::mat4(1.0f));
  }
}