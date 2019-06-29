/*
*  Author: Davide Viero - dviero42@gmail.com
*  Rha raytracer
*  2019
*  License: see LICENSE file
* 
*/

#include "sphere.h"

using namespace glm;

Sphere::Sphere(std::string name,
  vec3 pos,
  vec3 scale,
  vec3 col,
  float ka,
  float kd,
  float ks,
  float kr,
  float n
)
{
  this->name = name;
  coefficients.ka = ka;
  coefficients.kd = kd;
  coefficients.ks = ks;
  coefficients.kr = kr;
  coefficients.n = n;

  color = col;

  transform = mat4(
    vec4(scale[0], 0.0f, 0.0f, 0.0f),
    vec4(0.0f, scale[1], 0.0f, 0.0f),
    vec4(0.0f, 0.0f, scale[2], 0.0f),
    vec4(pos, 1.0f));
  invTransform = inverse(transform);
  invNormalsTransform = transpose(invTransform);
}

std::tuple<bool, std::vector<float>> Sphere::intersections(Ray r)
{
  // transformed direction in sphere space
  vec3 c1 = vec3(invTransform * vec4(r.getDirectionVector(), 0.0f));
  // transformed point in sphere space
  vec3 s1 = vec3(invTransform * vec4(r.getOriginVector(), 1.0f));

  float d = pow(dot(s1, c1), 2.0f) - dot(c1, c1) * (dot(s1, s1) - 1.0f);
  if (d < 0.0f)
    return std::make_tuple(false, std::vector<float> {0.0f});

  float t0 = (-dot(s1, c1) + sqrt(d)) / dot(c1, c1);
  float t1 = (-dot(s1, c1) - sqrt(d)) / dot(c1, c1);


  if (t0 > t1)
  {
    float swap = t0;
    t0 = t1;
    t1 = swap;
  }
  std::vector<float> intersectionsT = {t0,t1};
/* 
  if(t0>0.0001f || t1>0.0001f) return std::make_tuple(true, intersectionsT);
  else return std::make_tuple(false, intersectionsT);
  */
  return std::make_tuple(true, intersectionsT);
}