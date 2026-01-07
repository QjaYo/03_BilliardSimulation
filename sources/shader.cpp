#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "shader.h"
#include "light.h"
#include "material.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try
  {
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  }
  catch (std::ifstream::failure e)
  {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n"
      << "  vertexShaderPath = " << vertexPath << std::endl
      << "  fragmentShaderPath = " << fragmentPath << std::endl
      << "  what() = " << e.what()
      << std::endl;
  }

  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();

  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  this->ID = glCreateProgram();
  glAttachShader(this->ID, vertex);
  glAttachShader(this->ID, fragment);
  glLinkProgram(this->ID);
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::~Shader()
{
  glDeleteProgram(this->ID);
}

void Shader::use()
{
  glUseProgram(this->ID);
}

void Shader::setBool(const std::string &name, const bool value) const
{
  int loc = glGetUniformLocation(this->ID, name.c_str());
  glUniform1i(loc, (int)value);
}

void Shader::setInt(const std::string &name, const int value) const
{
  int loc = glGetUniformLocation(this->ID, name.c_str());
  glUniform1i(loc, value);
}

void Shader::setFloat(const std::string &name, const float value) const
{
  int loc = glGetUniformLocation(this->ID, name.c_str());
  glUniform1f(loc, value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
  int loc = glGetUniformLocation(this->ID, name.c_str());
  glUniform2fv(loc, 1, &value[0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
  int loc = glGetUniformLocation(this->ID, name.c_str());
  glUniform3fv(loc, 1, &value[0]);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
  int loc = glGetUniformLocation(this->ID, name.c_str());
  glUniform4fv(loc, 1, &value[0]);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
  int loc = glGetUniformLocation(this->ID, name.c_str());
  glUniformMatrix2fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
  int loc = glGetUniformLocation(this->ID, name.c_str());
  glUniformMatrix3fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
  int loc = glGetUniformLocation(this->ID, name.c_str());
  glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMaterial(const std::string &name, const Material &material) const
{
  setVec4(name + ".color", material.color);
  setVec3(name + ".ka", material.ka);
  setVec3(name + ".kd", material.kd);
  setVec3(name + ".ks", material.ks);
  setFloat(name + ".shininess", material.shininess);
  setBool(name + ".useDiffuseMaps", material.useDiffuseMaps);
}

void Shader::setLight(const std::string &name, const Light &light) const
{
  setVec4(name + ".position", glm::vec4(light.object.transform.position, 1.0f));
  if (light.object.render.material)
  {
    setVec4(name + ".color", light.object.render.material->color);
  }
  setFloat(name + ".intensity", light.intensity);
}