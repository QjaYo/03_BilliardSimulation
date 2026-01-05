#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "texture.h"

class Material
{
public:
  glm::vec4 color = glm::vec4(1.0f);
  glm::vec3 ka = glm::vec3(0.5f);
  glm::vec3 kd = glm::vec3(0.5f);
  glm::vec3 ks = glm::vec3(0.5f);
  float shininess = 1.0f;

  bool useDiffuseMaps = false;
  bool useSpecularMaps = false;
  bool useNormalMaps = false;
  bool useHeightMaps = false;

  std::vector<Texture *> diffuseMaps;
  std::vector<Texture *> specularMaps;
  std::vector<Texture *> normalMaps;
  std::vector<Texture *> heightMaps;
  std::vector<Texture *> otherMaps;

  Material(
    glm::vec4 c = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
    glm::vec3 a = glm::vec3(0.1f),
    glm::vec3 d = glm::vec3(0.5f),
    glm::vec3 s = glm::vec3(0.5f),
    float sh = 1.0f
  ) : color(c), ka(a), kd(d), ks(s), shininess(sh) {}

  void addTexture(Texture *texture);
  void activateDiffuseMaps();
  void activateSpecularMaps();
  void activateNormalMaps();
  void activateHeightMaps();
};