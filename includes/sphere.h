#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include "sceneObject.h"
#include "glm.hpp"

struct Coefficients {
  float ka;
  float kd;
  float ks;
  float kr;
  float n;
};

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

    std::tuple<bool, std::vector<float>> closestIntersection(Ray r) override;
    glm::vec3 computeNormal(glm::vec3 point);
    glm::vec3 localIllum(Ray r, float t, std::vector<Sphere *> spheres, std::vector<Light *> lights, glm::vec3 ambientColor);
    Coefficients coeff();
    glm::vec3 color();

    std::string name;
  private : 
    Coefficients coefficients;
    glm::vec3 sphereColor;
};