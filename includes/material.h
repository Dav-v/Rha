/*
*  Author: Davide Viero - dviero42@gmail.com
*  Rha raytracer
*  2019
*  License: see LICENSE file
* 
*/

#pragma once

#include "vec3.hpp"
#include <string>

class Material{
  public:
  Material() {};
  Material(
    std::string name,
    glm::vec3 ka,
    glm::vec3 kd,
    glm::vec3 ks,
    glm::vec3 kr,
    float n
  );
  Material(std::string name);

  glm::vec3 getKambient();
  glm::vec3 getKdiffuse();
  glm::vec3 getKspecular();
  glm::vec3 getKreflection();

private:
  std::string name;
  glm::vec3 Kambient;
  glm::vec3 Kdiffuse;
  glm::vec3 Kspecular;
  glm::vec3 Kreflection;
  float brightness;
};