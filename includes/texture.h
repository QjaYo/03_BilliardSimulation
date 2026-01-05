#pragma once

#include <string>
#include <vector>

class Texture
{
public:
  unsigned int ID;
  unsigned int target; // GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP, etc...
  std::string type; // AMBIENT, DIFFUSE, SPECULAR, NORMAL, HEIGHT, OTHER

  Texture(const std::string &typeName);
  ~Texture();

  void bind(unsigned int unit = 0) const;
  void load2D(const std::string &filePath);
  void load3D(const std::string &filePath);
  void loadCubeMap(const std::vector<std::string> &facePaths);
};