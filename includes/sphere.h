/*
*  Author: Davide Viero - dviero42@gmail.com
*  Rha raytracer
*  2019
*  License: see LICENSE file
* 
*/

#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include "sceneObject.h"
#include "glm.hpp"


class Sphere : public SceneObject{
  public:
    Sphere(std::string name,
      glm::vec3 position,
      glm::vec3 scale,
      glm::vec3 color,
      float ka,
      float kd,
      float ks,
      float kr,

      float n
    );

    std::tuple<bool, std::vector<float>> intersections(Ray r) override;

};