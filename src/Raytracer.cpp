/*
*  Author: Davide Viero - dviero42@gmail.com
*  Rha raytracer
*  2019
*  License: see LICENSE file
* 
*/

#include "raytracer.h"
#include <cstdlib>
#include <limits>
using namespace glm;

Raytracer::Raytracer(Scene scene)
{
  this->scene = scene;
}

std::tuple<bool,SceneObject*, float> Raytracer::intersections(Ray r)
{
  SceneObject *hitObject = NULL;
  float closestT = INFINITY;
  auto objects = scene.getObjects();
  auto finalResult = std::make_tuple(false, hitObject, closestT);
  for (unsigned int i = 0; i < objects.size(); i++)
  {
    auto intersections = objects[i]->intersections(r);
    if(std::get<bool>(intersections)){
      auto intersectionsVec = std::get<std::vector<float>>(intersections);
      for(auto& intersection :intersectionsVec)
      {
        if (intersection < closestT && intersection > 0.0001f)
        {
          closestT = intersection;
          hitObject = objects[i];
          std::get<bool>(finalResult) = true;
        }
      }
    }
  }
  std::get<SceneObject *>(finalResult) = hitObject;
  std::get<float>(finalResult) = closestT;
  return finalResult;
}

vec3 Raytracer::trace(Ray r)
{
  auto objects = scene.getObjects();
  auto lights = scene.getLights();
  if (r.getDepth() > 3)
    return vec3(0.0f);
  auto result = intersections(r);

  bool hasHitSomething = std::get<bool>(result);
  
  if (!hasHitSomething && r.getDepth() == 1) return scene.getBackground();
  if (!hasHitSomething) return vec3(0.0f);

  vec3 color;
  SceneObject *hitObject = std::get<SceneObject*>(result);

  float hitT = std::get<float>(result);
  color = hitObject->localIllum(r,hitT, objects, lights, scene.getAmbient());
  
  vec3 p = r.point(hitT);
  vec3 normal = hitObject->computeNormal(p);

  // reflect the ray
  vec3 newC = normalize(reflect(normalize(r.getDirectionVector()), normal));
  vec3 newS = p;

  color += hitObject->coeff().kr * trace(Ray(newC, newS, r.getDepth()+1));
  color = clamp(color, vec3(0.0f), vec3(1.0f));
  return abs(color);
}