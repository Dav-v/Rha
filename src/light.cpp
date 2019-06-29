#include "light.h"

using namespace glm;
Light::Light(std::string name,
               vec3 position,
               vec3 color
    )
{
  lightName = name;
  lightPosition = vec3(position);
  lightColor = vec3(color);
}

vec3 Light::position()
{
  return lightPosition;
};
vec3 Light::color()
{
  return lightColor;
}