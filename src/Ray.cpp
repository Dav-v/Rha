/*
*  Author: Davide Viero - dviero42@gmail.com
*  Rha raytracer
*  2019
*  License: see LICENSE file
* 
*/

#include "ray.h"

using namespace glm;

Ray::Ray(vec3 dir, vec3 p)
{
  c = dir;
  s = p;
}
Ray::Ray(vec3 dir, vec3 p, int d)
{
  c = dir;
  s = p;
  depth = d;
}

Ray::Ray(
  float near,
  float w,
  float h,
  int nCols,
  int nRows,
  int x,
  int y
)
{
  vec3 d(
      w * ((2.0f * static_cast<float> (y) ) / static_cast<float>(nCols) - 1.0f),
      h * ((2.0f * static_cast<float> (x) ) / static_cast<float>(nRows) - 1.0f),
      -near);
  c = vec3(d);
  s = vec3(d);
  depth = 1;
}

vec3 Ray::getOriginVector()
{
  return s;
}
vec3 Ray::getDirectionVector()
{
  return c;
}
int Ray::getDepth()
{
  return depth;
}
void Ray::setDepth(int d)
{
  depth = d;
}
vec3 Ray::point(float t)
{
  return c * t + s;
}