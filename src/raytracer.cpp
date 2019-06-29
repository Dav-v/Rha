#include "raytracer.h"
#include <cstdlib>
#include <limits>
using namespace glm;

Raytracer::Raytracer(vec3 ambient, vec3 back)
{
  backgroundColor = back;
  ambientColor = ambient;
}

std::tuple<bool,Sphere*, float> Raytracer::intersections(Ray r, std::vector<Sphere *> spheres)
{
  Sphere *hitSphere = NULL;
  float closestT = INFINITY;
  auto finalResult = std::make_tuple(false, hitSphere, closestT);
  for (unsigned int i = 0; i < spheres.size(); i++)
  {
    auto intersections = spheres[i]->closestIntersection(r);
    if(std::get<bool>(intersections)){
      auto intersectionsVec = std::get<std::vector<float>>(intersections);
      for(unsigned int k = 0; k < intersectionsVec.size();k++)
      {
        if (intersectionsVec[k] < closestT && intersectionsVec[k] > 0.0001f)
        {
          closestT = intersectionsVec[k];
          hitSphere = spheres[i];
          std::get<bool>(finalResult) = true;
        }
      }
    }
  }
  std::get<Sphere *>(finalResult) = hitSphere;
  std::get<float>(finalResult) = closestT;
  return finalResult;
}

vec3 Raytracer::trace(Ray r,std::vector<Sphere *> spheres, std::vector<Light *> lights)
{
  if (r.getDepth() > 3)
    return vec3(0.0f);
  auto result = intersections(r, spheres);

  bool hasHitSomething = std::get<bool>(result);
  
  if (!hasHitSomething && r.getDepth() == 1) return backgroundColor;
  if (!hasHitSomething) return vec3(0.0f);

  vec3 color;
  Sphere *hitSphere = std::get<Sphere*>(result);

  float hitT = std::get<float>(result);
  color = hitSphere->localIllum(r,hitT, spheres, lights, ambientColor);
  
  vec3 p = r.point(hitT);
  vec3 normal = hitSphere->computeNormal(p);

  // reflect the ray
  vec3 newC = normalize(reflect(normalize(r.getDirectionVector()), normal));
  vec3 newS = p;

  color += hitSphere->coeff().kr * trace(Ray(newC, newS, r.getDepth()+1),spheres, lights);
  color = clamp(color, vec3(0.0f), vec3(1.0f));
  return abs(color);
}