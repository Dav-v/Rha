#pragma once

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>

#include "sphere.h"
#include "light.h"
#include "ray.h"

class Raytracer
{
public:
  Raytracer(glm::vec3 ambient,glm::vec3 back);
  glm::vec3 point(float t);
  glm::vec3 trace(Ray r, std::vector<Sphere *> spheres, std::vector<Light *> lights);
  std::tuple<bool, Sphere *, float> intersections(Ray r, std::vector<Sphere *> spheres);

private:
  glm::vec3 backgroundColor;
  glm::vec3 ambientColor;
  int depth;
};