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
               float n)
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
  normalsTransform = transpose(transform);
  invTransform = inverse(transform);
  invNormalsTransform = transpose(invTransform);
}

std::tuple<bool, std::vector<float>> Sphere::intersections(Ray r)
{
  // transformed direction in sphere space
  vec3 c1 = vec3(invTransform * vec4(r.getDirectionVector(), 0.0f));
  // transformed point in sphere space
  vec3 s1 = vec3(invTransform * vec4(r.getOriginVector(), 1.0f));

  /*
  Equation for a ray intersection a sphere : 
  t^2 (dot(c1,c1)) + 2*t (dot(c1,s1)) + (dot(s1,s1) -1) = 0
   */
  float a = dot(c1, c1);
  float b = dot(s1, c1);
  float c = (dot(s1, s1) - 1.0f);
  
  float d = pow(b, 2.0f) - a * c;
  if (d < 0.0f)
    return std::make_tuple(false, std::vector<float>{0.0f});

  float t0 = (-b + sqrt(d)) / a;
  float t1 = (-b - sqrt(d)) / a;

  if (t0 > t1)
  {
    float swap = t0;
    t0 = t1;
    t1 = swap;
  }
  std::vector<float> intersectionsT = {t0, t1};
  /* 
  if(t0>0.0001f || t1>0.0001f) return std::make_tuple(true, intersectionsT);
  else return std::make_tuple(false, intersectionsT);
  */
  return std::make_tuple(true, intersectionsT);
}