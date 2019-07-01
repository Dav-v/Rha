/*
*  Author: Davide Viero - dviero42@gmail.com
*  Rha raytracer
*  2019
*  License: see LICENSE file
* 
*/

#include "plane.h"

using namespace glm;

Plane::Plane(std::string name,
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

std::tuple<bool, std::vector<float>> Plane::intersections(Ray r)
{
  // transformed direction in plane space
  vec3 c1 = vec3(invTransform * vec4(r.getDirectionVector(), 0.0f));
  // transformed point in plane space
  vec3 s1 = vec3(invTransform * vec4(r.getOriginVector(), 1.0f));

  /*
  Plane:
  z = 0
  -0.5 <= x <= 0.5
  -0.5 <= y <= 0.5
  Eq Plane:
  (x,y,z) = c1 * t + s1
  z = c1.z * t + s1.z

  Sol for z = 0
  0 = c1.z * t + s1.z
  -s1.z = c1.z * t
  t = -(s1.z)/c1.z
  x = c1.x * t + s1.x
  y = c1.y * t + s1.y

  if x and y are inside the boundaries, the plane is intersected
   */
  float t = -s1.y / c1.y;
  std::vector<float> intersectionsT = {t};

  float x = c1.x * t + s1.x;
  float z = c1.z * t + s1.z;
  if (
      (-0.5 <= x   ) &&
      ( x   <= 0.5 ) &&
      (-0.5 <= z   ) && 
      ( z   <= 0.5 )
    )
    return std::make_tuple(true, intersectionsT);
  else
    return std::make_tuple(false, intersectionsT);
}

vec3 Plane::computeNormal(vec3 point)
{
  return vec3(0.0f, 1.0f, 0.0f);
}