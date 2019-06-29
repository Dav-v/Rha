/*
*  Author: Davide Viero - dviero42@gmail.com
*  Rha raytracer
*  2019
*  License: see LICENSE file
* 
*/

#include "sceneObject.h"

using namespace glm;

vec3 SceneObject::computeNormal(vec3 point)
{
  vec3 invNormal = normalize(vec3(invTransform * vec4(point, 1.0f)));
  vec3 normal = normalize(vec3(invNormalsTransform * vec4(invNormal, 0.0f)));
  return normal;
}

vec3 SceneObject::localIllum(Ray r, float t, std::vector<SceneObject *> objects, std::vector<Light *> lights, vec3 ambientColor)
{
  vec3 p = r.point(t);
  vec3 normal = computeNormal(p);
  vec3 icolor = color * ambientColor * coefficients.ka;
  std::vector<Light *> illumLights;
  for (unsigned int i = 0; i < lights.size(); i++)
  {
    bool isVisible = true;
    vec3 dir = lights[i]->position() - p;

    for (unsigned int k = 0; k < objects.size(); k++)
    {
      std::tuple<bool, std::vector<float>> result = objects[k]->intersections(Ray(dir, p));
      if (std::get<bool>(result))
      {
        float t0 = std::get<std::vector<float>>(result)[0];
        float t1 = std::get<std::vector<float>>(result)[1];
        if ((t0 > 0.0001f && t0 < .9999f) ||
            (t1 > 0.0001f && t1 < .9999f))
        {
          isVisible = false;
          break;
        }
      }
    }
    if (isVisible)
      illumLights.push_back(lights[i]);
  }

  for (unsigned int i = 0; i < illumLights.size(); i++)
  {
    vec3 l = normalize(illumLights[i]->position() - p);
    // diffuse

    if (dot(normalize(r.getDirectionVector()), normal) <= 0.0f)
    {
      //return vec3(1.0, 0, 0);
      icolor += illumLights[i]->getColor() * coefficients.kd *
               (max(dot(normal, l), 0.0f) * color);
    }
    // specular
    vec3 v = normalize(r.getDirectionVector());
    vec3 ref = normalize(reflect(l, normal));

    icolor += illumLights[i]->getColor() * coefficients.ks *
             pow(clamp(dot(ref, v), 0.0f, 1.0f), coefficients.n);
  }
  return icolor;
}