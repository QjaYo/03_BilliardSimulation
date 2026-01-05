#pragma once

#include <glm/glm.hpp>
#include <string>

struct Light;
struct Material;

class Shader
{
public:
  unsigned ID; // program ID

  Shader(const char *vertexShaderPath, const char *fragmentShaderPath);
  ~Shader();

  void use();

  void setFloat(const std::string &name, const float value) const;
  void setBool(const std::string &name, const bool value) const;
  void setInt(const std::string &name, const int value) const;
  void setVec2(const std::string &name, const glm::vec2 &value) const;
  void setVec3(const std::string &name, const glm::vec3 &value) const;
  void setVec4(const std::string &name, const glm::vec4 &value) const;
  void setMat2(const std::string &name, const glm::mat2 &value) const;
  void setMat3(const std::string &name, const glm::mat3 &value) const;
  void setMat4(const std::string &name, const glm::mat4 &value) const;
  void setTexture2D(const std::string &name, unsigned int textureID) const;
  void setMaterial(const std::string &name, const Material &material) const;
  void setLight(const std::string &name, const Light &light) const;
};
