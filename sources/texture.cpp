#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <glad/glad.h>

Texture::Texture(const std::string &typeName)
  : ID(0), target(GL_TEXTURE_2D), type(typeName) {}

void Texture::load2D(const std::string &filePath)
{
  if (this->ID != 0)
    glDeleteTextures(1, &this->ID);

  this->target = GL_TEXTURE_2D;

  glGenTextures(1, &this->ID);
  glBindTexture(GL_TEXTURE_2D, this->ID);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
  if (data)
  {
    GLenum format;
    if (nrChannels == 1)
      format = GL_RED;
    else if (nrChannels == 3)
      format = GL_RGB;
    else if (nrChannels == 4)
      format = GL_RGBA;
    else
      std::cout << "Invalid texture format" << std::endl;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
  else
  {
    std::cout << "Texture failed to load at path: " << filePath << std::endl;
  }
  stbi_image_free(data);
}

void Texture::load3D(const std::string &filePath)
{}

void Texture::loadCubeMap(const std::vector<std::string> &facePaths)
{
  if (this->ID != 0)
    glDeleteTextures(1, &this->ID);

  this->target = GL_TEXTURE_CUBE_MAP;

  glGenTextures(1, &this->ID);
  glBindTexture(this->target, this->ID);

  int width, height, nrChannels;
  //stbi_set_flip_vertically_on_load(true);
  for (unsigned int i = 0;i < facePaths.size();i++)
  {
    unsigned char *data = stbi_load(facePaths[i].c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
      std::cout << "Successfully loaded: " << facePaths[i] << std::endl;
      GLenum format;
      if (nrChannels == 1)
        format = GL_RED;
      else if (nrChannels == 3)
        format = GL_RGB;
      else if (nrChannels == 4)
        format = GL_RGBA;
      else
        std::cout << "Invalid texture format" << std::endl;
      
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }
    else
    {
      std::cout << "CubeMap failed to load at: " << facePaths[i] << std::endl;
    }
    stbi_image_free(data);
  }

  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

Texture::~Texture()
{
  glDeleteTextures(1, &ID);
}

void Texture::bind(unsigned int unit) const {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(this->target, ID);
}