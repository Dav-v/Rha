/*
*  Author: Davide Viero - dviero42@gmail.com
*  Rha raytracer
*  2019
*  License: see LICENSE file
* 
*/

#pragma once
#include <tuple>
#include "material.h"

#include "ray.h"
#include "light.h"
#include "mat4x4.hpp"
#include "vec3.hpp"

// TODO use Material class
struct Coefficients
{
  float ka;
  float kd;
  float ks;
  float kr;
  float n;
};

class SceneObject{
  public:
    virtual std::tuple<bool, std::vector<float>> intersections(Ray r) = 0;
    virtual glm::vec3 computeNormal(glm::vec3 point);
    virtual glm::vec3 localIllum(Ray r, float t, std::vector<SceneObject *> objects, std::vector<Light *> lights, glm::vec3 ambientColor);

    glm::mat4 getTransform() { return transform; };
    glm::mat4 getInvTransform() { return invTransform; };
    glm::mat4 getInvNormalsTransform() { return invNormalsTransform; };
    Coefficients coeff() { return coefficients; };
    glm::vec3 getColor() { return color; };
  protected:
    glm::mat4 transform = glm::mat4(1.0f);
    glm::mat4 normalsTransform = glm::mat4(1.0f);
    glm::mat4 invTransform = glm::mat4(1.0f);
    glm::mat4 invNormalsTransform = glm::mat4(1.0f);
    Material material;
    Coefficients coefficients;
    glm::vec3 color;
    std::string name;
};