#include "material.h"

void Material::addTexture(Texture *texture)
{
  if (texture->type == "DIFFUSE")
    this->diffuseMaps.push_back(texture);
  else if (texture->type == "SPECULAR")
    this->specularMaps.push_back(texture);
  else if (texture->type == "NORMAL")
    this->normalMaps.push_back(texture);
  else if (texture->type == "HEIGHT")
    this->heightMaps.push_back(texture);
  else
    this->otherMaps.push_back(texture);
}

void Material::activateDiffuseMaps()
{
  this->useDiffuseMaps = true;
}

void Material::activateSpecularMaps()
{
  this->useSpecularMaps = true;
}

void Material::activateNormalMaps()
{
  this->useNormalMaps = true;
}

void Material::activateHeightMaps()
{
  this->useHeightMaps = true;
}