/*
*  Author: Davide Viero - dviero42@gmail.com
*  Rha raytracer
*  2019
*  License: see LICENSE file
* 
*/

#include "light.h"

using namespace glm;
Light::Light(std::string name,
      vec3 position,
      vec3 color
    )
{
  lightName = name;
  lightPosition = position;
  lightColor = color;
}

vec3 Light::position()
{
  return lightPosition;
};
vec3 Light::getColor()
{
  return lightColor;
}